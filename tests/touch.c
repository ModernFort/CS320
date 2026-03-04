#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "../src/commands/touch.h"

void no_file() {
  CU_ASSERT_EQUAL(touch(NULL, 0), -1);
  char *a[] = {"-a"};
  CU_ASSERT_EQUAL(touch(*a, 1), -1);
  char *m[] = {"-m"};
  CU_ASSERT_EQUAL(touch(*m, 1), -1);
  char *am[] = {"-am"};
  CU_ASSERT_EQUAL(touch(*am, 1), -1);
  char *ma[] = {"-ma"};
  CU_ASSERT_EQUAL(touch(*ma, 1), -1);
  char *am_sep[] = {"-a", "-m"};
  CU_ASSERT_EQUAL(touch(*am_sep, 2), -1);
  char *ma_sep[] = {"-m", "-a"};
  CU_ASSERT_EQUAL(touch(*ma_sep, 2), -1);
}

void black_box() {
  no_file();
  // file exists
  // file does not exist
  // file accessible
  // file not accessible
}

int main() {
  CU_initialize_registry();

  CU_pSuite blackBox = CU_add_suite("Black Box Tests", NULL, NULL);
  CU_pSuite whiteBox = CU_add_suite("White Box Tests", NULL, NULL);
  CU_pSuite integration = CU_add_suite("Integration Tests", NULL, NULL);
  // black box test cases:
  // run tests
  // cleanup registry
  return CU_get_error();
}
