#ifndef HISTORY_H
#define HISTORY_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define CMD_SIZE 256 // max command size that can be cached (trivial)
#define HIST_SIZE 50 // max size of history buffer (trivial, overwrites oldest commands on overflow)

/**
 * @brief Acts as data structure to store history commands
 * 
 * The commands array stores pointer to strings. It has is programmed in such a way that it has
 * circular properties. It has a trivial max size which can be viewed/adjusted in the 'history.c' 
 * file. All functions have been written to accommodate the circular buffer and it's behavior.
 */
typedef struct {
  int size;           /**< Total number of commands stored (cannot exceed max size) */
  int head;           /**< Current location of next open slot (able to wrap around buffer) */
  int tail;           /** marks the last/oldest command (also wraps like head) */
  int curIndex;       /**< Used for subsequent use of lastHist() commands */
  char *commands[HIST_SIZE];   /**< storage of pointers to command strings (size set by macro in C file) */
} History;

/**
 * @brief Initializes a history struct where command history will be cached with all field
 * s zeroed out, along with any necessary metadata
 * 
 * @return initialized history object
 * @warning Caller is responsible for clearing memory allocated by this function
 */
History *initHist(void);

/**
 * @brief Adds command to history buffer
 *
 * command is added by allocating new string onto the heap. The pointer is added to this 
 * history buffer.
 *
 * @param h      Pointer to History struct
 * @param input  Command string to cache
 * @return size of history buffer on success, -1 if there was an error
 * @warning Caller is respoinsible for clearing memory allocated by this function 
 * (in freeHist or clearHist)
 */
int cacheHist(History *h, char *input);


/**
 * @brief Display full history of commands entered, up to max size of buffer
 * 
 * @param h Pointer to History struct
 * @return 0 on success, -1 if there was an error
 */
int fullHist(History *h);

/**
 * @brief Displays previous history command entered, if it exists
 * 
 * @param h Pointer to history struct
 * @return the previous history command
 */
char *lastHist(History *h);

/**
 * @brief Displays next history command entered, if it exists
 * 
 * @param h Pointer to history struct
 * @return the next history command
 */
char *nextHist(History *h);

/**
 * @brief resets curIndex pointer to 0 so the next LastHist restarts at the head
 * 
 * @param h Pointer to history struct
 * @return 0 on success, -1 on error
 */
int resetCurIndex(History *h);

/**
 * @brief completely frees history object and any associated fields
 * 
 * @param h Pointer to history struct
 * @return 0 on success, -1 on error
 * @warning This should only be used in the case that the user is closing the program/tab
 */
int freeHist(History *h);

/**
 * @brief Resets the command list stored in histroy
 * 
 * @param h Pointer to the History struct
 * @return 0 on success, -1 on error
 */
int clearHist(History *h);

/**
 * @brief prints last x commands that have been entered
 * 
 * @param h Pointer to the History struct
 * @param num number of commands to print
 * @return 0 on success, -1 on error
 */
int lastXHist(History *h, int num);


/**
 * @brief matches format of parser method to take in args and proc accordingly
 * 
 * @param cmdParams list of arguments following history
 * @param paramsCount size of list
 * @param h Pointer to the History struct
 * @param num number of commands to 
 * 
 */
void history(char **cmdParams, int paramsCount, History *h);


#endif
