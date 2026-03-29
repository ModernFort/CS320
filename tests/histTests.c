#include <criterion/criterion.h>
#include "history.h"

/**
 * WHITEBOX TESTING- 100% Branch coverage
 * 
 *  int lastHist(History *h) {
 *  // check empty list
 *  if (h->size == 0) return -1;
 * 
 *  // hard code check for edge case of looping array
 *  int prevIndex = (h->head == 0) ? HIST_SIZE - 1 : h->head - 1;
 * 
 *  printf("%s\n", h->commands[prevIndex]);
 *  
 *  return 0;
 *  } 
 * 
 */
Test(whitebox_testing, lastHist_branches) {
  // make 3 history ojbects
  History *h1 = initHist();
  History *h2 = initHist();
  History *h3 = initHist();

  // no loading for h1 because testing empty list


  // load h2 full so head == 0 but not empty
  for (int i = 0; i < HIST_SIZE; i++) {
    cacheHist(h2, "test input");
  }

  // load h3 partially for final case
  for (int i = 0; i < 10; i++) {
    cacheHist(h3, "test input");
  }
  


  // run tests
  cr_assert_eq(lastHist(h1), -1, "Incorrect handling of running lastHist on empty list");
  cr_assert_eq(lastHist(h2), 0, "Incorrect handling of running lastHist where head == 0");
  cr_assert_eq(lastHist(h3), 0, "Incorrect handling of running lastHist on regular history list");

  freeHist(h1);
  freeHist(h2);
  freeHist(h3);
}

/**
 * INTEGRATION TESTING
 * 
 * testing units: cacheHist, clearHist, freeHist
 * 
 * Note: from my understanding, even though these are in the same file, they are still external 
 * because they will be used directly in main/not static. 
 */
Test(integration_testing, caching_and_clearing_history) {
  History *h1 = initHist();

  // cache inputs
  for (int i = 0; i < 5; i++) {
    cacheHist(h1, "test input:");
  }

  // asserts caching works correctly
  cr_assert_eq(h1->size, 5, "Incorrectly handles caching/size tracking");

  // now try to clear them
  clearHist(h1);

  cr_assert_eq(h1->size, 0, "Incorrectly handles size when clearing data");
  cr_assert_eq(h1->head, 0, "Incorrectly handles head when clearing data");
  cr_assert_eq(h1->commands[0], NULL, "Incorrectly handles removing commands when clearing data");

  // may as well make sure freeHist works too
  // assumes freeHist is error free && will propery free every time. For guarantee, use valgrind
  cr_assert_eq(freeHist(h1), 0, "Incorrectly handles freeing History struct");
}

/**
 * BLACK BOX TESTING
 * 
 * ensuring circular buffer used for storing commands works correctly at all possible points
 */
Test(black_box_testing, caching_logic) {
  History *h1 = initHist();

  // add command, make sure size, and value update correctly
  cr_assert_eq(cacheHist(h1, "T3ST 1NpUT"), 1, "Incorrectly handles caching history/scaling size");
  cr_assert_str_eq(h1->commands[0], "T3ST 1NpUT", "Incorrectly handles head when adding data");

  // fill to brim and ensure everything has been added correctly
  for (int i = 0; i < HIST_SIZE - 1; i++) { cacheHist(h1, "test input"); }
  cr_assert_eq(h1->size, HIST_SIZE, "Incorrectly handles size when maxing command buffer");
  cr_assert_str_eq(h1->commands[0], "T3ST 1NpUT", "Incorrectly handled adding around max buffer");
  cr_assert_str_eq(h1->commands[HIST_SIZE -1], "test input", "Incorrectly handled adding around max buffer");

  // test circulaity by overflowing/forcing circular properties
  cacheHist(h1, "overflowing now");
  cr_assert_eq(h1->size, HIST_SIZE, "Incorrectly handles size when forcing circularity");
  cr_assert_str_eq(h1->commands[0], "overflowing now", 
                  "Incorrectly handles updating commands when forcing circularity");

  freeHist(h1);
}