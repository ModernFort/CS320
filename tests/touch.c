#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
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

void file_create_a() {
  file_creation_test_prep();
  char *opts[] = {"test.tmp", "-a"};
  CU_ASSERT_EQUAL(touch(opts, 2), 0);
  file_creation_test_cleanup();
}

void file_create_m() {
  file_creation_test_prep();
  char *opts[] = {"test.tmp", "-m"};
  CU_ASSERT_EQUAL(touch(opts, 2), 0);
  file_creation_test_cleanup();
}

void file_create_am() {
  file_creation_test_prep();
  char *opts[] = {"test.tmp" "-am"};
  CU_ASSERT_EQUAL(touch(opts, 2), 0);
  file_creation_test_cleanup();
}

void file_create_ma() {
  file_creation_test_prep();
  char *opts[] = {"test.tmp", "-ma"};
  CU_ASSERT_EQUAL(touch(opts, 2), 0);
  file_creation_test_cleanup();
}

void file_create_am_sep() {
  file_creation_test_prep();
  char *opts[] = {"test.tmp", "-a", "-m"};
  CU_ASSERT_EQUAL(touch(opts, 3), 0);
  file_creation_test_cleanup();
}

void file_create_ma_sep() {
  file_creation_test_prep();
  char *opts[] = {"test.tmp", "-m", "-a"};
  CU_ASSERT_EQUAL(touch(opts, 3), 0);
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

void existing_accessible_no_opt() {
  if (existing_accessible_prep() == -1) {
    return;
  }
  time_t before[4];
  if (retrieve_mod_and_acc(before) == -1) {
    // retrieval failed
    return;
  }
  char *opts[] = {"test.tmp"};
  CU_ASSERT_EQUAL(touch(opts, 1), 0);
  time_t after[4];
  if (retrieve_mod_and_acc(after) == -1) {
    // retrieval failed
    return;
  }
  CU_ASSERT(before[0] != after[0] || before[1] != after[1]);
  CU_ASSERT(before[2] != after[2] || after[3] != after[3]);
  existing_accessible_cleanup();
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

void existing_accessible_m() {
  if (existing_accessible_prep() == -1) {
    return;
  }
  time_t before[4];
  if (retrieve_mod_and_acc(before) == -1) {
    // retrieval failed
    return;
  }
  char *opts[] = {"test.tmp", "-m"};
  CU_ASSERT_EQUAL(touch(opts, 2), 0);
  time_t after[4];
  if (retrieve_mod_and_acc(after) == -1) {
    // retrieval failed
    return;
  }
  CU_ASSERT(before[0] != after[0] || before[1] != after[1]);
  CU_ASSERT(before[2] == after[2] && after[3] == after[3]);
  existing_accessible_cleanup();
}

void existing_accessible_am() {
  if (existing_accessible_prep() == -1) {
    return;
  }
  time_t before[4];
  if (retrieve_mod_and_acc(before) == -1) {
    // retrieval failed
    return;
  }
  char *opts[] = {"test.tmp", "-am"};
  CU_ASSERT_EQUAL(touch(opts, 2), 0);
  time_t after[4];
  if (retrieve_mod_and_acc(after) == -1) {
    // retrieval failed
    return;
  }
  CU_ASSERT(before[0] != after[0] || before[1] != after[1]);
  CU_ASSERT(before[2] != after[2] || after[3] != after[3]);
  existing_accessible_cleanup();
}

void existing_accessible_ma() {
  if (existing_accessible_prep() == -1) {
    return;
  }
  time_t before[4];
  if (retrieve_mod_and_acc(before) == -1) {
    // retrieval failed
    return;
  }
  char *opts[] = {"test.tmp", "-ma"};
  CU_ASSERT_EQUAL(touch(opts, 2), 0);
  time_t after[4];
  if (retrieve_mod_and_acc(after) == -1) {
    // retrieval failed
    return;
  }
  CU_ASSERT(before[0] != after[0] || before[1] != after[1]);
  CU_ASSERT(before[2] != after[2] || after[3] != after[3]);
  existing_accessible_cleanup();
}

void existing_accessible_am_sep() {
  if (existing_accessible_prep() == -1) {
    return;
  }
  time_t before[4];
  if (retrieve_mod_and_acc(before) == -1) {
    // retrieval failed
    return;
  }
  char *opts[] = {"test.tmp", "-a", "-m"};
  CU_ASSERT_EQUAL(touch(opts, 3), 0);
  time_t after[4];
  if (retrieve_mod_and_acc(after) == -1) {
    // retrieval failed
    return;
  }
  CU_ASSERT(before[0] != after[0] || before[1] != after[1]);
  CU_ASSERT(before[2] != after[2] || after[3] != after[3]);
  existing_accessible_cleanup();
}

void existing_accessible_ma_sep() {
  if (existing_accessible_prep() == -1) {
    return;
  }
  time_t before[4];
  if (retrieve_mod_and_acc(before) == -1) {
    // retrieval failed
    return;
  }
  char *opts[] = {"test.tmp", "-m", "-a"};
  CU_ASSERT_EQUAL(touch(opts, 3), 0);
  time_t after[4];
  if (retrieve_mod_and_acc(after) == -1) {
    // retrieval failed
    return;
  }
  CU_ASSERT(before[0] != after[0] || before[1] != after[1]);
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

void existing_inaccessible_a() {
  if (existing_inaccessible_prep() == -1) {
    return;
  }
  char *opts[] = {"test.tmp", "-a"};
  CU_ASSERT_EQUAL(touch(opts, 2), -1);
  existing_inaccessible_cleanup();
}

void existing_inaccessible_m() {
  if (existing_inaccessible_prep() == -1) {
    return;
  }
  char *opts[] = {"test.tmp", "-m"};
  CU_ASSERT_EQUAL(touch(opts, 2), -1);
  existing_inaccessible_cleanup();
}

void existing_inaccessible_am() {
  if (existing_inaccessible_prep() == -1) {
    return;
  }
  char *opts[] = {"test.tmp", "-am"};
  CU_ASSERT_EQUAL(touch(opts, 2), -1);
  existing_inaccessible_cleanup();
}

void existing_inaccessible_ma() {
  if (existing_inaccessible_prep() == -1) {
    return;
  }
  char *opts[] = {"test.tmp", "-am"};
  CU_ASSERT_EQUAL(touch(opts, 2), -1);
  existing_inaccessible_cleanup();
}

void existing_inaccessible_am_sep() {
  if (existing_inaccessible_prep() == -1) {
    return;
  }
  char *opts[] = {"test.tmp", "-a", "-m"};
  CU_ASSERT_EQUAL(touch(opts, 3), -1);
  existing_inaccessible_cleanup();
}

void existing_inaccessible_ma_sep() {
  if (existing_inaccessible_prep() == -1) {
    return;
  }
  char *opts[] = {"test.tmp", "-m", "-a"};
  CU_ASSERT_EQUAL(touch(opts, 3), -1);
  existing_inaccessible_cleanup();
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
  add_test(blackBox, "Touch test.tmp", file_create_no_opt);
  add_test(blackBox, "Touch test.tmp -a", file_create_a);
  add_test(blackBox, "Touch test.tmp -m", file_create_m);
  add_test(blackBox, "Touch test.tmp -am", file_create_am);
  add_test(blackBox, "Touch test.tmp -ma", file_create_ma);
  add_test(blackBox, "Touch test.tmp -a -m", file_create_am_sep);
  add_test(blackBox, "Touch test.tmp -m -a", file_create_ma_sep);
  // file specified exists and is accessible
  add_test(blackBox, "Existing touch test.tmp", existing_accessible_no_opt);
  add_test(blackBox, "Existing touch test.tmp -a", existing_accessible_a);
  add_test(blackBox, "Existing touch test.tmp -m", existing_accessible_m);
  add_test(blackBox, "Existing touch test.tmp -am", existing_accessible_am);
  add_test(blackBox, "Existing touch test.tmp -ma", existing_accessible_ma);
  add_test(blackBox, "Existing touch test.tmp -a -m", existing_accessible_am_sep);
  add_test(blackBox, "Existing touch test.tmp -m -a", existing_accessible_ma_sep);
  // file specified exists but is inaccessible
  add_test(blackBox, "Inaccessible touch test.tmp", existing_inaccessible_no_opt);
  add_test(blackBox, "Inaccessible touch test.tmp -a", existing_inaccessible_a);
  add_test(blackBox, "Inaccessible touch test.tmp -m", existing_inaccessible_m);
  add_test(blackBox, "Inaccessible touch test.tmp -am", existing_inaccessible_am);
  add_test(blackBox, "Inaccessible touch test.tmp -ma", existing_inaccessible_ma);
  add_test(blackBox, "Inaccessible touch test.tmp -a -m", existing_inaccessible_am_sep);
  add_test(blackBox, "Inaccessible touch test.tmp -m -a", existing_inaccessible_ma_sep);
  // run tests
  CU_cleanup_registry();
  return CU_get_error();
}
