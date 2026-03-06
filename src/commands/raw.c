#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

// yes, this must be global, at least within the file
static struct termios defaultTermios;

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

  // set stdin to the new raw files, 
  // middle flag flushes buffer before applting so keystrokes aren't lost 
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

}

