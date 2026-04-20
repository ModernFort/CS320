#ifndef RAW_H
#define RAW_H

#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./../history/history.h"


#define MAX_SEQ_SIZE 7

/**
 * @brief All necessary information for the terminal to run, including a history struct, a command
 * buffer, it's size, a pointer to where the user is in the buffer, and a tracker to know if the
 * terminal is still running
 */
typedef struct {
  History *hist;         /** < pointer to history struct */
  char cmd[CMD_SIZE];    /** < command buffer */
  int cmdCurs;           /** < cursor to move around in the command buffer */
  int cmdSize;           /** < current length of command */
  int live;              /** < status of terminal */
} Context;


/**
 * @brief DIY tuple/hashmap tracking (ANSI escape sequence -> function)
 */
typedef struct {
  const char *keySeq;         /** < ANSI char seq  */
  void (*handler)(Context*);  /** < pointer to handler function  */
} KeyMap;


/**
 * @brief handles up arrow keystroke.
 * Retrieves previous command (if exists), replaces current command buffer with it, and updates
 * terminal screen
 * 
 * @param context pointer to context
 */
void handleUARR(Context *context);


/**
 * @brief handles down arrow keystroke.
 * Retrieves next command (if exists), replaces current command buffer with it, and updates
 * terminal screen
 * 
 * @param context pointer to context
 */
void handleDARR(Context *context);


/**
 * @brief handles left arrow keystroke.
 * Moves cursor in terminal context and updates screen
 * 
 * @param context pointer to context
 */
void handleLARR(Context *context);

/**
 * @brief handles left arrow keystroke.
 * Moves cursor in terminal context and updates screen
 * 
 * @param context pointer to context
 */
void handleRARR(Context *context);

/**
 * @brief handles regular keystroke.
 * inserts character at location of cursor (updates all buffers accordingly and visually)
 * 
 * @param context pointer to context
 * @param c character to be inserted
 */
void handleCHAR(Context *context, char c);

/**
 * @brief handles backspace keystroke.
 * deletes character at location of cursor in buffer, updates all relevant metadata and screen
 * 
 * @param context pointer to context
 */
void handleBACK(Context *context);

/**
 * @brief handles enter keystroke.
 * sends command into parser, updates all context metadata, yada yada yada
 * 
 * @param context pointer to context
 */
void handleENTR(Context *context);

/**
 * @brief initializes all fields in Context struct to zeroed out values
 * 
 * @return context struct
 */
Context *initContext();


/**
 * @brief restores terminal to original state before we started messing around with stuff
 * 
 * Run automatically on program exit
 */
void exitRaw();

/**
 * @brief changes terminal settings from default into our custom settings
 * Specifically, turns off termios character echoing and canonical key handling mode
 * Also, updates os return settings so we can interpret individual keystrokes as commands
 * 
 * Run automatically on program exit
 */
void enterRaw();

/**
 * @brief Run if ANSI escape character is entered. We will read which one it was by recreating
 * string from it's input, then run it if it has a matching key
 * 
 * @param context pointer to terminal metadata
 */
void parseEscapeKeys(Context *context);

/**
 * @brief processes every keystroke, runs correspoinding method based on what is input
 * 
 * @param context pointer to terminal metadata
 */
void procInputs(Context *context);

#endif
