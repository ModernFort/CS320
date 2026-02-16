#define CMD_SIZE 256
#define HIST_SIZE 50

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

typedef struct {
  int size; 
  int head;
  char *commands[HIST_SIZE];
} History;


History *initHist() {
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
  if (h->commands[h->head] != NULL) free(h->commands[h->head]);

  h->commands[h->head] = temp; 

  // increment head and size, but account for max HIST_SIZE
  h->head = (h->head + 1) % HIST_SIZE;
  h->size = (h->size == HIST_SIZE) ? HIST_SIZE : h->size + 1;  

  return h->size;
} 

int fullHist(History *h) {
  int start = (h->size < HIST_SIZE) ? 0 : h->head;

  for (int i = start; i < (start + h->size); i++) {
    printf("%s\n", h->commands[i % HIST_SIZE]);
  }
  return 0;
}

int lastHist(History *h) {
  // check empty list
  if (h->size == 0) return -1;

  // hard code check for edge case of looping array
  int prevIndex = (h->head == 0) ? HIST_SIZE - 1 : h->head - 1;

  printf("%s\n", h->commands[prevIndex]);
  
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
  return 0;
}
