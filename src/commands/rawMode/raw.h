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
  int cmdSize;
  int live;
} Context;


// TODO: learn this shit more
typedef void (*KeyHandler)(Context*);

void handleUARR(Context *context);

void handleDARR(Context *context);

void handleLARR(Context *context);

void handleRARR(Context *context);

void handleCHAR(Context *context, char c);

void handleBACK(Context *context);

void handleENTR(Context *context);



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




Context *initContext();

void exitRaw();

void enterRaw();

void parseEscapeKeys(Context *context);

void procInputs(Context *context);

#endif
