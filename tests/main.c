#include <check.h>

#include "../s21_decimal.h"
#include "s21_decimal_tests.h"

void runtests() {
  Suite *all_functions[] = {suite_s21_add_test(),
                            suite_s21_sub_test(),
                            suite_s21_mul_test(),
                            suite_s21_div_test(),
                            suite_s21_less_test(),
                            suite_s21_less_or_equal_test(),
                            suite_s21_greater_test(),
                            suite_s21_greater_or_equal_test(),
                            suite_s21_equal_test(),
                            suite_s21_not_equal_test(),
                            suite_s21_from_int_to_decimal_test(),
                            suite_s21_from_float_to_decimal_test(),
                            suite_s21_from_decimal_to_int_test(),
                            suite_s21_from_decimal_to_float_test(),
                            suite_s21_floor_test(),
                            suite_s21_round_test(),
                            suite_s21_truncate_test(),
                            suite_s21_negate_test(),
                            NULL};

  for (int i = 0; all_functions[i] != NULL; i++) {
    runtest(all_functions[i]);
  }
}

void runtest(Suite *test) {
  SRunner *sr = srunner_create(test);
  srunner_run_all(sr, CK_NORMAL);
  int num_failed = srunner_ntests_failed(sr);
  if (num_failed == 0) {
    printf("All tests passed\n");
  } else {
    printf("Number of failed tests: %d\n", num_failed);
  }
  srunner_free(sr);
}

int main() {
  runtests();
  return 0;
}