#include "s21_decimal.h"

int is_greater(s21_decimal num1, s21_decimal num2,
               char variant) {  // 0-(нет) 1-(да) variant-(для состояния работы,
                                // при включенном, игнорирует знак)
  int is_true = 0;
  if (variant) {
    set_sign(&num1, 0);
    set_sign(&num2, 0);
  }
  int sign1 = get_sign(num1);
  int sign2 = get_sign(num2);
  if (sign1 < sign2) {
    is_true = 1;
  } else if (sign1 == sign2) {
    int flag = 0;
    scale_equalize(&num1, &num2);
    for (int i = 95; i >= 0 && !flag; i--) {
      int bit1 = get_bit(num1, i);
      int bit2 = get_bit(num2, i);
      if (bit1 != bit2) {
        is_true = !sign1 ? (bit1 > bit2) : (bit1 < bit2);
        flag = 1;
      }
    }
  }
  return is_true;
}

int is_less(s21_decimal num1, s21_decimal num2, char variant) {
  char is_true = 0;
  if (variant) {
    set_sign(&num1, 0);
    set_sign(&num2, 0);
  }
  char sign1 = get_sign(num1);
  char sign2 = get_sign(num2);
  if (sign1 > sign2) {
    is_true = 1;
  } else if (sign1 == sign2) {
    char flag = 0;
    scale_equalize(&num1, &num2);
    for (int i = 95; i >= 0 && !flag; i--) {
      int bit1 = get_bit(num1, i);
      int bit2 = get_bit(num2, i);
      if (bit1 != bit2) {
        is_true = !sign1 ? (bit1 < bit2) : (bit1 > bit2);
        flag = 1;
      }
    }
  }
  return is_true;
}

int is_equal(s21_decimal num1, s21_decimal num2, char variant) {
  char is_true =
      1;  // изначально оба числа равны, потом значение меняется или не меняется
  if (zero_check(num1) && zero_check(num2)) {
    is_true = 1;
  } else if (((get_sign(num1) != get_sign(num2) && !variant)) ||
             (get_scale(&num1) != get_scale(&num2) && !variant)) {
    is_true = 0;
  } else {
    char flag = 0;
    for (int i = 95; i >= 0 && !flag; i--) {
      if (get_bit(num1, i) != get_bit(num2, i)) {
        is_true = 0;
        flag = 1;
      }
    }
  }
  return is_true;
}

int is_greater_or_equal(s21_decimal num1, s21_decimal num2, char variant) {
  int is_true =
      (is_greater(num1, num2, variant) || is_equal(num1, num2, variant));
  return is_true;
}

int is_less_or_equal(s21_decimal num1, s21_decimal num2, char variant) {
  int is_true = (is_less(num1, num2, variant) || is_equal(num1, num2, variant));
  return is_true;
}

// дальше идут функции, чтобы чисто подходили под таску

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int res = is_less(value_1, value_2, 0);
  return res;
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  int res = is_less_or_equal(value_1, value_2, 0);
  return res;
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  int res = is_greater(value_1, value_2, 0);
  return res;
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  int res = is_greater_or_equal(value_1, value_2, 0);
  return res;
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int res = is_equal(value_1, value_2, 0);
  return res;
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_equal(value_1, value_2);
}
