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
} Context;

Context *initContext(){
  Context *c = (Context*)calloc(1, sizeof(Context));
  c->hist = initHist();
  c->cmdCurs = 0;
  return c;
}

// enum of multi-character keys (easily scalable)
typedef enum {
  KEY_NORMAL,
  KEY_UARR,
  KEY_DARR,
  KEY_RARR,
  KEY_LARR
} Keystroke;

// struct to link ('string' -> keystroke)
typedef struct {
  const char *keySeq;
  Keystroke key;
} KeyMap;

// list of links (easily scalable)
static KeyMap binds[] = {
  {"\x1b[A", KEY_UARR},
  {"\x1b[B", KEY_DARR},
  {"\x1b[C", KEY_RARR},
  {"\x1b[D", KEY_LARR}
};



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

// TODO: learn this shit more
typedef void (*KeyHandler)(Context*);

void handleUARR(Context *context) {
  char *prevCmd = lastHist(context->hist);
  if (prevCmd != NULL){
    strcpy(context->cmd, prevCmd);
    context->cmdCurs = strlen(context->cmd);
    printf("\r\x1b[2K😎👉 %s", prevCmd);
    fflush(stdout);
  }
}

void handleDARR(Context *c) {
    printf("\r\n[DEBUG] Moving Down in History...");
}

// TODO: wtf, learn this shit more
KeyHandler dispatchTable[] = {
    [KEY_UARR]    = handleUARR,
    [KEY_DARR]  = handleDARR,
    [KEY_LARR]  = NULL, // We can fill these in later
    [KEY_RARR] = NULL
};




void parseEscapeKeys(Context *context) {
  int counter = 1;
  char seq[MAX_SEQ_SIZE+2] = {0}; // + 1 for escape char, +1 for null term
  seq[0] = '\x1b';
  
  while (counter <= MAX_SEQ_SIZE && read(STDIN_FILENO, seq + counter, 1) == 1) {
    counter++;
  }

  // TODO: 4 is length of 'binds' for now. We fix hard coding later
  for (int i = 0; i < 4; i++) {
    if(strcmp(seq, binds[i].keySeq) == 0) {
      int action = binds[i].key;
      if (dispatchTable[action] != NULL) dispatchTable[action](context);
      fflush(stdout);
    }
  }
}

void procInputs(Context *context) {
  char c;
  printf("😎👉 ");
  fflush(stdout);
  while (1) {

    // read a single char from stdin
    int input = read(STDIN_FILENO, &c, 1);
    if (input == 1) {

      // TODO: we'll probably want to handle backspace first so it get's procd before anything else

      // quit character... TODO: Change quit input later
      if (c == 'q') break;

      
      else if (c == '\n' || c == '\r') {
        // cache history command
        cacheHist(context->hist, context->cmd);

        //TODO: actually execute whatever gets called here, if possible
        printf("\r\nRunning: %s\r\n", context->cmd);

        // reset cmd buffer
        memset(context->cmd, 0, CMD_SIZE);
        context->cmdCurs = 0;

        printf("😎👉 ");
        fflush(stdout);
      }
      
      // if you press special keys (that require escape char) proc accordingly
      else if (c == '\x1b') { parseEscapeKeys(context); }

      // otherwise, it's a normal character, so just print to stdout
      else {
        context->cmd[context->cmdCurs] = c;
        context->cmdCurs++;
        printf("%c", c);
        fflush(stdout);
      }
    }
  }
}

int main() {

  enterRaw();

  Context *c = initContext();

  printf("Now entering raw mode. Normal terminal shortcuts may not work. Press 'q' to exit\r\n");

  procInputs(c);

  return 0;
}
