#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <unistd.h>
#include <stdlib.h>
#include "../src/commands/touch.h"

void no_file_null() {
  CU_ASSERT_EQUAL(touch(NULL, 0), -1);
}

void no_file_a() {
  char *a[] = {"-a"};
  CU_ASSERT_EQUAL(touch(a, 1), -1);
}

void no_file_m() {
  char *m[] = {"-m"};
  CU_ASSERT_EQUAL(touch(m, 1), -1);
}

void no_file_am() {
  char *am[] = {"-am"};
  CU_ASSERT_EQUAL(touch(am, 1), -1);
}

void no_file_ma() {
  char *ma[] = {"-ma"};
  CU_ASSERT_EQUAL(touch(ma, 0), -1);
}

void no_file_am_sep() {
  char *am_sep[] = {"-a", "-m"};
  CU_ASSERT_EQUAL(touch(am_sep, 1), -1);
}

void no_file_ma_sep() {
  char *ma_sep[] = {"-m", "-a"};
  CU_ASSERT_EQUAL(touch(ma_sep, 2), -1);
}

void file_creation_test_prep() {
  if (access("test.txt", F_OK) == 0) {
    // file exists
    if (remove("test.txt") != 0) {
      // file removal failed
      CU_FAIL("Remove: Could not prepare environment for testing");
    }
  }
}

void file_creation_test_cleanup() {
  if (access("test.txt", F_OK) != 0) {
    // file doesn't exist
    CU_FAIL("touch test.txt: Did not create file");
  } else {
    if (remove("test.txt") != 0) {
      // file removal failed
      CU_FAIL("Remove: Could not cleanup environment");
    }
  }
}

void file_create_no_opt() {
  file_creation_test_prep();
  char *opts[] = {"test.txt"};
  CU_ASSERT_EQUAL(touch(opts, 1), 0);
  file_creation_test_cleanup();
}

void file_create_a() {
  file_creation_test_prep();
  char *opts[] = {"test.txt", "-a"};
  CU_ASSERT_EQUAL(touch(opts, 2), 0);
  file_creation_test_cleanup();
}

void file_create_m() {
  file_creation_test_prep();
  char *opts[] = {"test.txt", "-m"};
  CU_ASSERT_EQUAL(touch(opts, 2), 0);
  file_creation_test_cleanup();
}

void file_create_am() {
  file_creation_test_prep();
  char *opts[] = {"test.txt" "-am"};
  CU_ASSERT_EQUAL(touch(opts, 2), 0);
  file_creation_test_cleanup();
}

void file_create_ma() {
  file_creation_test_prep();
  char *opts[] = {"test.txt", "-ma"};
  CU_ASSERT_EQUAL(touch(opts, 2), 0);
  file_creation_test_cleanup();
}

void file_create_am_sep() {
  file_creation_test_prep();
  char *opts[] = {"test.txt", "-a", "-m"};
  CU_ASSERT_EQUAL(touch(opts, 3), 0);
  file_creation_test_cleanup();
}

void file_create_ma_sep() {
  file_creation_test_prep();
  char *opts[] = {"test.txt", "-m", "-a"};
  CU_ASSERT_EQUAL(touch(opts, 3), 0);
  file_creation_test_cleanup();
}

void add_test(CU_pSuite suite, const char *name, CU_TestFunc func) {
  if (CU_add_test(suite, name, func) == NULL) {
    fprintf(stderr, "Error: Unable to add test '%s' to the suite. Error code: %d\n", name, CU_get_error());
    CU_cleanup_registry();
    exit(EXIT_FAILURE);
  }
}

int main() {
  CU_initialize_registry();

  CU_pSuite blackBox = CU_add_suite("Black Box Tests", NULL, NULL);
  CU_pSuite whiteBox = CU_add_suite("White Box Tests", NULL, NULL);
  CU_pSuite integration = CU_add_suite("Integration Tests", NULL, NULL);
  // black box test cases
  // no file specified
  add_test(blackBox, "Touch", no_file_null);
  add_test(blackBox, "Touch -a", no_file_a);
  add_test(blackBox, "Touch -m", no_file_m);
  add_test(blackBox, "Touch -am", no_file_am);
  add_test(blackBox, "Touch -ma", no_file_ma);
  add_test(blackBox, "Touch -a -m", no_file_am_sep);
  add_test(blackBox, "Touch -m -a", no_file_ma_sep);
  // file specified does not exist
  add_test(blackBox, "Touch test.txt", file_create_no_opt);
  add_test(blackBox, "Touch test.txt -a", file_create_a);
  add_test(blackBox, "Touch test.txt -m", file_create_m);
  add_test(blackBox, "Touch test.txt -am", file_create_am);
  add_test(blackBox, "Touch test.txt -ma", file_create_ma);
  add_test(blackBox, "Touch test.txt -a -m", file_create_am_sep);
  add_test(blackBox, "Touch test.txt -m -a", file_create_ma_sep);
  // run tests
  CU_cleanup_registry();
  return CU_get_error();
}
