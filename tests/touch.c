#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include "../src/commands/touch.h"

void file_creation_test_prep() {
  if (access("test.tmp", F_OK) == 0) {
    // file exists
    if (remove("test.tmp") != 0) {
      // file removal failed
      CU_FAIL("Remove: Could not prepare environment for testing");
    }
  }
}

void file_creation_test_cleanup() {
  if (remove("test.tmp") != 0) {
    // file removal failed
    CU_FAIL("Remove: Could not cleanup environment");
  }
}

void file_create_no_opt() {
  file_creation_test_prep();
  char *opts[] = {"test.tmp"};
  CU_ASSERT_EQUAL(touch(opts, 1), 0);
  file_creation_test_cleanup();
}

void file_create_am() {
  file_creation_test_prep();
  char *opts[] = {"test.tmp" "-am"};
  CU_ASSERT_EQUAL(touch(opts, 2), 0);
  file_creation_test_cleanup();
}

int existing_accessible_prep() {
  int fd = mkstemp("test.tmp");
  if (fd == -1) {
    // file creation failed
    CU_FAIL("Mkstmep: Could not prepare environment for testing.");
    return -1;
  } else {
    close(fd);
    // file created
    return 1;
  }
}

int retrieve_mod_and_acc(time_t *times) {
  struct stat file_stat;
  if (stat("test.tmp", &file_stat) < 0) {
    CU_FAIL("Stat: Could not retrieve file information");
    return -1;
  }
  times[0] = file_stat.st_atim.tv_sec;
  times[1] = file_stat.st_atim.tv_nsec;
  times[2] = file_stat.st_mtim.tv_sec;
  times[3] = file_stat.st_mtim.tv_nsec;
  return 0;
}

void existing_accessible_cleanup() {
  if (remove("test.tmp") != 0) {
    // file removal failed
    CU_FAIL("Remove: Could not cleanup environment");
  }
}

void existing_accessible_a() {
  if (existing_accessible_prep() == -1) {
    return;
  }
  time_t before[4];
  if (retrieve_mod_and_acc(before) == -1) {
    // retrieval failed
    return;
  }
  char *opts[] = {"test.tmp", "-a"};
  CU_ASSERT_EQUAL(touch(opts, 2), 0);
  time_t after[4];
  if (retrieve_mod_and_acc(after) == -1) {
    // retrieval failed
    return;
  }
  CU_ASSERT(before[0] == after[0] && before[1] == after[1]);
  CU_ASSERT(before[2] != after[2] || after[3] != after[3]);
  existing_accessible_cleanup();
}

int remove_write() {
  struct stat file_stat;

  if (stat("test.tmp", &file_stat) < 0) {
    CU_FAIL("Stat: Could not prepare environment for testing");
    return -1;
  }

  mode_t newPermissions = file_stat.st_mode & ~(S_IWUSR | S_IWGRP | S_IWOTH);

  if (chmod("test.tmp", newPermissions) < 0) {
    CU_FAIL("Stat: Could not prepare environment for testing");
    return -1;
  }
  return 0;
}

int existing_inaccessible_prep() {
  int fd = mkstemp("test.tmp");
  if (fd == -1) {
    // file creation failed
    CU_FAIL("Mkstmep: Could not prepare environment for testing.");
    return -1;
  } else {
    close(fd);
    // file created
    remove_write();
    return 1;
  }
}

void existing_inaccessible_cleanup() {
  if (remove("test.tmp") != 0) {
    // file removal failed
    CU_FAIL("Remove: Could not cleanup environment");
  }
}

void existing_inaccessible_no_opt() {
  if (existing_inaccessible_prep() == -1) {
    return;
  }
  char *opts[] = {"test.tmp"};
  CU_ASSERT_EQUAL(touch(opts, 1), -1);
  existing_inaccessible_cleanup();
}

void invalid_opt() {
  if (existing_accessible_prep() == -1) {
    return;
  }
  char *opts[] = {"test.tmp", "-s"};
  CU_ASSERT_EQUAL(touch(opts, 2), -1);
  existing_accessible_cleanup();
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
  add_test(blackBox, "Touch test.tmp", file_create_no_opt);
  add_test(blackBox, "Existing touch test.tmp -a", existing_accessible_a);
  add_test(blackBox, "Inaccessible touch test.tmp", existing_inaccessible_no_opt);
  add_test(blackBox, "Touch test.tmp -s", invalid_opt);
  // whitebox
  // the following test, along with the blackbox tests provide statement coverage of touch()
  add_test(whiteBox, "Touch test.tmp -am", file_create_am);
  // no file specified
  // run tests
  CU_cleanup_registry();
  return CU_get_error();
}
