#ifndef S21_DECIMAL
#define S21_DECIMAL

#include <math.h>
#include <stdio.h>

#define FAIL 1;
#define SUCCESS 0;

typedef enum {
  NORMAL = 0,     // нормальное число
  PLUS_INF = 1,   // было плюсовое переполнение
  MINUS_INF = 2,  // было минусовое переполнение
  ZERO_SUB = 3    // было деление на ноль
} value_conditions;

typedef struct {
  int bits[4];
  value_conditions value_condition;
} s21_decimal;

// base
int get_bit(s21_decimal num, int bit);
void set_bit(s21_decimal *num, int bit, int val);
int get_sign(s21_decimal num);
void set_sign(s21_decimal *num, int sign);
int get_scale(const s21_decimal *num);
void set_scale(s21_decimal *num, int scale);
int find_high_bit(s21_decimal num1);
int scale_equalize(s21_decimal *num1, s21_decimal *num2);
void calc_diff(s21_decimal num1, s21_decimal num2, s21_decimal *res);
void offset_left(s21_decimal *num, int offset_num);
void offset_right(s21_decimal *num, int offset_num);
int zero_check(s21_decimal num);
s21_decimal num_check(s21_decimal num1, s21_decimal num2);
s21_decimal bit_add(s21_decimal num1, s21_decimal num2);
s21_decimal bit_sub(s21_decimal num1, s21_decimal num2);
s21_decimal bit_mul(s21_decimal num1, s21_decimal num2);
s21_decimal bit_10mul(s21_decimal num, int scale);
s21_decimal bit_div(s21_decimal num1, s21_decimal num2, s21_decimal *ostatok);

// compare
int is_greater(s21_decimal num1, s21_decimal num2, char variant);
int is_greater_or_equal(s21_decimal num1, s21_decimal num2, char variant);
int is_less(s21_decimal num1, s21_decimal num2, char variant);
int is_less_or_equal(s21_decimal num1, s21_decimal num2, char variant);
int is_equal(s21_decimal num1, s21_decimal num2, char variant);
int s21_is_less(s21_decimal value_1, s21_decimal value_2);
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);

// calculations
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// other
int s21_negate(s21_decimal value, s21_decimal *result);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);

// convert
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

#endif