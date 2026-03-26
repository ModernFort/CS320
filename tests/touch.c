#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include "../src/commands/touch.h"
#include "../src/commands/ls.h"

/**
 * helper to any file creation tests
 * for test prep
 * checks if tmp file is present and deletes if necessary
 * function triggers fail if removal unsuccessful
*/
void file_creation_test_prep() {
  if (access("test.tmp", F_OK) == 0) {
    // file exists
    if (remove("test.tmp") != 0) {
      // file removal failed
      CU_FAIL("Remove: Could not prepare environment for testing");
    }
  }
}

/**
 * helper to any file creation tests
 * for test cleanup
 * removes tmp file and triggers fail if removal unsuccessful
  */
void file_creation_test_cleanup() {
  if (remove("test.tmp") != 0) {
    // file removal failed
    CU_FAIL("Remove: Could not cleanup environment");
  }
}

/**
 * @Test
 * Tests touch() where file specified does not exist
 * No options specified
  */
void file_create_no_opt() {
  file_creation_test_prep();
  char *opts[] = {"test.tmp"};
  CU_ASSERT_EQUAL(touch(opts, 1), 0);
  CU_ASSERT_EQUAL(access("test.tmp", F_OK), 0);
  file_creation_test_cleanup();
}

/**
 * @Test
 * Tests touch() where file specified does not exist
 * options: -am
  */
void file_create_am() {
  file_creation_test_prep();
  char *opts[] = {"test.tmp", "-am"};
  CU_ASSERT_EQUAL(touch(opts, 2), 0);
  CU_ASSERT_EQUAL(access("test.tmp", F_OK), 0);
  file_creation_test_cleanup();
}

/**
 * helper method to tests involving an existing, accessible file
 * assumes tmp file does not exist
 * creates tmp file
 *
 * @returns 1 on success, -1 on error
  */
int existing_accessible_prep() {
  FILE *fd = fopen("test.tmp", "w");
  if (fd == NULL) {
    // file creation failed
    CU_FAIL("Fopen: Could not prepare environment for testing.");
    return -1;
  } else {
    fclose(fd);
    // file created
    return 1;
  }
}

/**
 * helper method retrieves access and modify times for tmp file,
 * stores them in array pointed to by times
 *
 * @param times - a pointer to the first element of a 4-element array of time_t
 *
 * @returns 0 on success or -1 on failure
  */
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

/**
 * helper to tests involving an existing, accessible file
 * removes tmp file, and fails test if removal fails
  */
void existing_accessible_cleanup() {
  if (remove("test.tmp") != 0) {
    // file removal failed
    CU_FAIL("Remove: Could not cleanup environment");
  }
}

/**
 * @Test
 * tests the command "touch test.tmp -a", where tmp file
 * exists and is accessible
 * confirms access time updated
  */
void existing_accessible_a() {
  if (existing_accessible_prep() == -1) {
    return;
  }
  time_t before[4];
  if (retrieve_mod_and_acc(before) == -1) {
    // retrieval failed
    return;
  }
  char *opts[] = {"-a", "test.tmp"};
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

/**
 * @Test
 * tests the command "touch test.tmp", where tmp file
 * exists and is inaccessible
 * confirms access and modification times updated
  */
void existing_inaccessible_no_opt() {
  int fd = mkstemp("test.tmp");
  if (fd == -1) {
    // file creation failed
    CU_FAIL("Mkstmep: Could not prepare environment for testing.");
    return;
  } else {
    close(fd);
    // file created
    struct stat file_stat;

    if (stat("test.tmp", &file_stat) < 0) {
      CU_FAIL("Stat: Could not prepare environment for testing");
      return;
    }

    mode_t newPermissions = file_stat.st_mode & ~(S_IWUSR | S_IWGRP | S_IWOTH);

    if (chmod("test.tmp", newPermissions) < 0) {
      CU_FAIL("Stat: Could not prepare environment for testing");
      return;
    }
  }

  char *opts[] = {"test.tmp"};
  CU_ASSERT_EQUAL(touch(opts, 1), -1);
  if (remove("test.tmp") != 0) {
    // file removal failed
    CU_FAIL("Remove: Could not cleanup environment");
  }
}

/**
 * Method finds search_string in file 'filename'
 *
 * @param filename - a pointer to the name of the file to be searched
 * @param search_string - a pointer to the string for which to search
 *
 * @returns 1 or 0 if the file contains the search string or not
 */
int find_string_in_file(const char *filename, const char *search_string) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    CU_FAIL("Ls Output Check: Error opening file");
    return -1;
  }

  char line[255];

  while (fgets(line, sizeof(line), file)) {
    if (strstr(line, search_string) != NULL) {
      fclose(file);
      return 1;
    }
  }
  
  fclose(file);
  return 0;
}

/**
 * @Test
 * tests touch() when an invalid option is passed
  */
void invalid_opt() {
  if (existing_accessible_prep() == -1) {
    return;
  }

  FILE *temp_file = fopen("output.tmp", "w");
  if (temp_file == NULL) {
      CU_FAIL("Failed to open output.tmp");
      return;
  }

  FILE *original_stderr = stderr;
  stderr = temp_file;

  char *opts[] = {"test.tmp", "-s"};
  CU_ASSERT_EQUAL(touch(opts, 2), -1);

  stderr = original_stderr;
  fclose(temp_file);

  CU_ASSERT_EQUAL(find_string_in_file("output.tmp", "touch: invalid option -- '-s'"), 0);

  existing_accessible_cleanup();
}


/**
 * @Test
 * integration test between touch and ls
 */
void touch_ls() {
  char *opts[] = {"test.tmp"};
  CU_ASSERT_EQUAL(touch(opts, 1), 0);

  FILE *temp_file = fopen("output.tmp", "w");
  if (temp_file == NULL) {
      CU_FAIL("Failed to open output.tmp");
      return;
  }

  FILE *original_stdout = stdout;
  stdout = temp_file;

  CU_ASSERT_EQUAL(ls(NULL), 0);

  stdout = original_stdout;
  fclose(temp_file);

  CU_ASSERT_EQUAL(find_string_in_file("output.tmp", "test.tmp"), 0);

  remove("output.tmp");
}

/**
 * helper function to add test "name" to suite "suite"
 * performs requisite error checking
 *
 * @param suite - the test suite to which to add the test
 * @param name - a pointer to a string which is the name of the test
 * @param func - the test function pointer
  */
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
  // add_test(blackBox, "Inaccessible touch test.tmp", existing_inaccessible_no_opt);
  // add_test(blackBox, "Touch test.tmp -s", invalid_opt);
  // the following test, along with the blackbox tests provide statement coverage of touch()
  // add_test(whiteBox, "Touch test.tmp -am", file_create_am);
  // add_test(integration, "Touch and ls", touch_ls);
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
