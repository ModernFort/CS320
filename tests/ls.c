#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdlib.h>
#include "../src/commands/ls.h"

void no_path() {
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

  temp_file = fopen("output.tmp", "r");
  if (temp_file == NULL) {
      CU_FAIL("Failed to open output.tmp");
      return;
  }

  char output[4096];
  fgets(output, sizeof(output), temp_file);
  fclose(temp_file);

  CU_ASSERT_STRING_EQUAL(output, "Hello, World!\n");

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
  // black box test cases
  add_test(blackBox, "Ls No Path", no_path);

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
