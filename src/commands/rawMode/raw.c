#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_SEQ_SIZE 7

// yes, this must be global, at least within the file
static struct termios defaultTermios;

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
}


void parseEscapeKeys() {
  int counter = 1;
  char seq[MAX_SEQ_SIZE+2] = {0}; // + 1 for escape char, +1 for null term
  seq[0] = '\x1b';
  
  while (counter <= MAX_SEQ_SIZE && read(STDIN_FILENO, seq + counter, 1) == 1) {
    //char cur = seq[counter];
    counter++;

    // per ANSI standard, any escape sequence will end with a char in this range. 
    // We will save input delay exiting here instead of waiting .1 seconds to proc final input
    //TODO: add this back if you have time/delay is noticable
    // if (cur >= '@' && cur <= '~') break;
  }

// TODO: 4 is length of 'binds' for now. We fix hard coding later
  for (int i = 0; i < 4; i++) {
    if(strcmp(seq, binds[i].keySeq) == 0) {
      printf("%d", binds[i].key);
      fflush(stdout);
    }
  }
}

void procInputs() {
  char c;
  while (1) {

    
    int input = read(STDIN_FILENO, &c, 1);
    if (input == 1) {
      if (c == 'q') break;
      else if (c == '\n' || c == '\r') {
        printf("\r\n");
        fflush(stdout);
      }
      
      else if (c == '\x1b') { parseEscapeKeys(); }
      else {
        printf("%c", c);
        fflush(stdout);
      }
    }
  }
}

int main() {

  enterRaw();

  printf("Now entering raw mode. Normal terminal shortcuts may not work. Press 'q' to exit\r\n");

  procInputs();

  return 0;
}
