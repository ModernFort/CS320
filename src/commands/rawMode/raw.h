#ifndef RAW_H
#define RAW_H

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./../history/history.h"

#define MAX_SEQ_SIZE 7

typedef struct {
  History *hist;
  char cmd[CMD_SIZE];
  int cmdCurs;
} Context;

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

// TODO: learn this shit more
typedef void (*KeyHandler)(Context*);

void handleUARR(Context *context);

void handleDARR(Context *c);

// TODO: wtf, learn this shit more
KeyHandler dispatchTable[] = {
    [KEY_UARR]    = handleUARR,
    [KEY_DARR]  = handleDARR,
    [KEY_LARR]  = NULL, // We can fill these in later
    [KEY_RARR] = NULL
};

Context *initContext();

void exitRaw();

void enterRaw();

void parseEscapeKeys(Context *context);

void procInputs(Context *context);

#endif
