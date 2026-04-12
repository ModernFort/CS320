#include "history.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


History *initHist(void) {
  // alloc history and sets everything to null/0
  History *h = (History*)calloc(1, sizeof(History));
  if (h == NULL) {
    fprintf(stderr, "Error: %s\n", strerror(errno));
    return NULL;
  }

  return h;
}

// here, we want to add something to our history pointer
int cacheHist(History *h, char *input) {
  // alloc new string on heap
  char *temp = (char*)malloc(strlen(input) + 1);
  if (temp == NULL) {
    fprintf(stderr, "Error: %s\n", strerror(errno));
    return -1;
  }

  strcpy(temp, input);

  // account for overwriting old commands
  if (h->commands[h->head] != NULL) {
    free(h->commands[h->head]);
    h->tail = (h->tail + 1) % HIST_SIZE;
  }

  h->commands[h->head] = temp; 

  // increment head and size, but account for max HIST_SIZE
  h->head = (h->head + 1) % HIST_SIZE;
  h->curIndex = h->head;
  h->size = (h->size == HIST_SIZE) ? HIST_SIZE : h->size + 1;  

  return h->size;
} 

int fullHist(History *h) {
  int start = h->tail;

  for (int i = start; i < (start + h->size); i++) {
    printf("%s\n", h->commands[i % HIST_SIZE]);
  }
  return 0;
}

char *lastHist(History *h) {
  // check empty list
  if (h->size == 0) return NULL;

  // so we don't keep going in circle if we reach end
  if (h->curIndex == h->tail) return h->commands[h->curIndex];

  // hard code check for edge case of looping array
  h->curIndex = (h->curIndex == 0) ? HIST_SIZE - 1 : h->curIndex - 1;
  
  // new return to accommodate raw mode
  return h->commands[h->curIndex];
}

// like inverse of lastHist... goes forward instead
char *nextHist(History *h) {
  if (h->size == 0) return NULL;

  // check to see if we are at head (for when not at full capacity)
  if (h->curIndex == h->head) return "";

  // get next cmd
  h->curIndex = h->curIndex == HIST_SIZE - 1 ? 0 : h->curIndex + 1;

  // check again to see if we are at head (for when we have started looping)
  if (h->curIndex == h->head) return "";

  return h->commands[h->curIndex];
}

int restCurIndex(History *h) {
  h->curIndex = h->head;
  return 0;
}

int freeHist(History *h) {
  for (int i = 0; i < h->size; i++) {
    free(h->commands[i]);
  }
  free(h);
  return 0;
}

int clearHist(History *h) {
  for (int i = 0; i < h->size; i++) {
    free(h->commands[i]);
    h->commands[i] = NULL;
  }
  h->size = 0;
  h->head = 0;
  h->tail = 0;
  h->curIndex = 0;
  return 0;
}
