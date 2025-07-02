#ifndef S21_DECIMAL_TESTS_H
#define S21_DECIMAL_TESTS_H

#include <check.h>
#include <ctype.h>
#include <locale.h>
#include <stdint.h>
#include <string.h>
#include <wchar.h>

#include "../s21_decimal.h"

#define S21_TRUE 1
#define S21_FALSE 0

#define BUFF_SIZE 512

void runtests();
void runtest(Suite *test);

Suite *suite_s21_add_test(void);
Suite *suite_s21_sub_test(void);
Suite *suite_s21_mul_test(void);
Suite *suite_s21_div_test(void);
Suite *suite_s21_less_test(void);
Suite *suite_s21_less_or_equal_test(void);
Suite *suite_s21_greater_test(void);
Suite *suite_s21_greater_or_equal_test(void);
Suite *suite_s21_equal_test(void);
Suite *suite_s21_not_equal_test(void);
Suite *suite_s21_from_int_to_decimal_test(void);
Suite *suite_s21_from_float_to_decimal_test(void);
Suite *suite_s21_from_decimal_to_int_test(void);
Suite *suite_s21_from_decimal_to_float_test(void);
Suite *suite_s21_floor_test(void);
Suite *suite_s21_round_test(void);
Suite *suite_s21_truncate_test(void);
Suite *suite_s21_negate_test(void);

#endif