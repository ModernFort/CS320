#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./../history/history.h"

#define MAX_SEQ_SIZE 7


// yes, this must be global, at least within the file
static struct termios defaultTermios;

typedef struct {
  History *hist;
  char cmd[CMD_SIZE];
  int cmdCurs;
  int cmdSize;
  int live;
} Context;

Context *initContext(){
  Context *c = (Context*)calloc(1, sizeof(Context));
  c->hist = initHist();
  c->cmdCurs = 0;
  c->cmdSize = 0;
  c->live = 1;
  return c;
}





void exitRaw() {
  tcsetattr(STDIN_FILENO, TCSANOW, &defaultTermios);
  printf("\x1b[0 q");
}

void enterRaw() {
  // take settings from stdin, save to default termios struct
  tcgetattr(STDIN_FILENO, &defaultTermios);

  // now if we exit at any point, we will rever to original terminal settings
  atexit(exitRaw);

  // copy old to our new one. Now we will change it. 
  struct termios raw = defaultTermios;

  // c_lflag is basicallu just unsigned int, and each bit represents info about the terminal
  // turning off echo will stop the terminal from printing all of our keystrokes
  raw.c_lflag &= ~ECHO;

  // turns on raw mode. Now we have to process literally every keystroke manually
  raw.c_lflag &= ~ICANON;

  // tell OS we can return 0 characters from read (by default, will be like 4 or sum due to stale
  // values from canonical mode)
  raw.c_cc[VMIN] = 0;

  // tell OS to wait maximum of 100ms before returning 0 to read. (Will no longer block for ever)
  raw.c_cc[VTIME] = 1;

  // set stdin to the new raw files, 
  // middle flag flushes buffer before applting so keystrokes aren't lost 
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
  printf("\x1b[6 q");
}


void handleUARR(Context *context) {
  char *prevCmd = lastHist(context->hist);
  if (prevCmd != NULL){
    strcpy(context->cmd, prevCmd);
    context->cmdCurs = strlen(context->cmd);
    context->cmdSize = strlen(context->cmd);
    printf("\r\x1b[2K😎👉 %s", prevCmd);
    fflush(stdout);
  }
}

void handleDARR(Context *context) {
  char *nextCmd = nextHist(context->hist);
  if (nextCmd != NULL) {
    strcpy(context->cmd, nextCmd);
    context->cmdCurs = strlen(context->cmd);
    context->cmdSize = strlen(context->cmd);
    printf("\r\x1b[2K😎👉 %s", nextCmd);
    fflush(stdout);
  }
}

void handleRARR(Context *context) {
  if (context->cmdCurs == context->cmdSize) return;
  printf("\x1b[C");
  context->cmdCurs++;
  fflush(stdout);
}

void handleLARR(Context *context) {
  if (context->cmdCurs == 0) return;
  printf("\x1b[D");
  context->cmdCurs--;
  fflush(stdout);
}

void handleBACK(Context *context) {
  if (context->cmdSize == 0 || context->cmdCurs == 0) return;

  for (int i = context->cmdCurs - 1; i < context->cmdSize; i++) {
    context->cmd[i] = context->cmd[i + 1];
  }

  context->cmdCurs--;
  context->cmdSize--;

  printf("\r\x1b[2K😎👉 %s", context->cmd);

   for (int i = context->cmdCurs; i < context->cmdSize; i++) {
    printf("\x1b[D");
  }

  fflush(stdout);
}

void handleCHAR(Context *context, char c) {
  // FULL
  if (context->cmdSize == CMD_SIZE) return;

  // easy case (added to end)
  if (context->cmdSize == context->cmdCurs) {
    context->cmd[context->cmdCurs] = c;
    context->cmdCurs++;
    context->cmdSize++;
    printf("%c", c);
    fflush(stdout);
    return;
  }

  // pain case
  // scoot everything over one, then insert
  for (int i = context->cmdSize; i > context->cmdCurs; i--) {
    context->cmd[i] = context->cmd[i - 1];
  }
  context->cmd[context->cmdCurs] = c;
  context->cmdSize++;
  context->cmdCurs++;
  printf("\r😎👉 %s", context->cmd);

  // move the cursor back where it's supposed to be
  for (int i = context->cmdSize; i > context->cmdCurs; i--) {
    printf("\x1b[D");
  }
  fflush(stdout);
}

void handleENTR(Context *context) {

  // handling exiting
  if (strcmp(":q", context->cmd) == 0) {
    printf("\nbye👋\n");
    fflush(stdout);
    context->live = 0;
    return;
  }

  // otherwise, we good
  cacheHist(context->hist, context->cmd);

  //TODO: actually execute whatever gets called here, if possible
  //printf("\r\nRunning: %s\r\n", context->cmd);

  // reset cmd buffer
  memset(context->cmd, 0, CMD_SIZE);
  context->cmdCurs = 0;
  context->cmdSize = 0;
  printf("\n\r😎👉 ");
  fflush(stdout);
}

// DIY special tuple
// struct to link (input sequence -> action to do)
typedef struct {
  const char *keySeq;
  void (*handler)(Context*);
} KeyMap;

// list of links (easily scalable)
static KeyMap binds[] = {
  {"\x1b[A", handleUARR},
  {"\x1b[B", handleDARR},
  {"\x1b[C", handleRARR},
  {"\x1b[D", handleLARR}
};


void parseEscapeKeys(Context *context) {
  int counter = 1;
  char seq[MAX_SEQ_SIZE+2] = {0}; // + 1 for escape char, +1 for null term
  seq[0] = '\x1b';
  
  // create full escape char from input
  while (counter <= MAX_SEQ_SIZE && read(STDIN_FILENO, seq + counter, 1) == 1) {
    counter++;
  }

  // check my binds to see if we have match
  int bindsLen = sizeof(binds) / sizeof(binds[0]);
  for (int i = 0; i < bindsLen; i++) {
    if(strcmp(seq, binds[i].keySeq) == 0) {
      if (binds[i].handler != NULL) binds[i].handler(context);
      //fflush(stdout);
    }
  }
}

void procInputs(Context *context) {
  char c;
  printf("😎👉 ");
  fflush(stdout);
  while (context->live) {

    // read a single char from stdin
    int input = read(STDIN_FILENO, &c, 1);
    if (input == 1) {

      // backspace for some reasion its just 127
      if (c == 127) handleBACK(context);
      
      // 
      else if (c == '\n' || c == '\r') handleENTR(context);
      
      // if you press special keys (that require escape char) proc accordingly
      else if (c == '\x1b') parseEscapeKeys(context);

      // otherwise, it's a normal character, so just print to stdout
      else handleCHAR(context, c);
    }
  }
}

int main() {

  enterRaw();

  Context *c = initContext();

  printf("Now entering custom terminal. Normal terminal shortcuts may not work. enter ':q' to exit\r\n");

  procInputs(c);

  return 0;
}
