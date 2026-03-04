#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <../src/touch.h>

int main() {
  CU_initialize_registry();

  CU_pSuite blackBox = CU_add_suite("Black Box Tests", NULL, NULL);
  CU_pSuite whiteBox = CU_add_suite("White Box Tests", NULL, NULL);
  CU_pSuite integration = CU_add_suite("Integration Tests", NULL, NULL);
  // black box test cases:
  // possible inputs:
  // touch
  // touch -a
  // touch -am
  // touch -ma
  // touch -m
  // touch -m -a
  // touch -a -m
  // file exists
  // file does not exist
  // file accessible
  // file not accessible
  // run tests
  // cleanup registry
  return CU_get_error();
}
