#include "s21_decimal.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  *result = value;
  int decision = FAIL;
  if (result != NULL) {
    get_sign(value) ? set_sign(result, 0) : set_sign(result, 1);
    decision = SUCCESS;
  }
  return decision;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int decision = FAIL;
  if (result != NULL && value.value_condition == NORMAL) {
    if (!get_scale(&value)) {
      *result = value;
    } else {
      int sign = get_sign(value);
      set_sign(&value, 0);
      s21_decimal ten = {{10, 0, 0, 0}, NORMAL};
      s21_decimal non_usable = {0};
      for (int i = get_scale(&value); i > 0; i--) {
        value = bit_div(value, ten, &non_usable);
      }
      *result = value;
      set_sign(result, sign);
      set_scale(result, 0);
    }
    decision = SUCCESS;
  }
  return decision;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  int decision = FAIL;
  if (result != NULL && value.value_condition == NORMAL) {
    if (!get_scale(&value)) {
      *result = value;
    } else {
      s21_decimal one = {{1, 0, 0, 0}, NORMAL};
      s21_truncate(value, result);
      if (get_sign(value)) {
        *result = bit_add(*result, one);
        set_sign(result, 1);
      }
    }
    decision = SUCCESS;
  }
  return decision;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  int decision = FAIL;
  if (result != NULL && value.value_condition == NORMAL) {
    if (!get_scale(&value)) {
      *result = value;
    } else {
      s21_decimal half = {{5, 0, 0, 0}, NORMAL};
      s21_decimal ten = {{10, 0, 0, 0}, NORMAL};
      s21_decimal one = {{1, 0, 0, 0}, NORMAL};
      s21_decimal value_copy = value;
      s21_decimal truncated = {0};
      s21_decimal ostatok = {0};
      s21_truncate(value_copy, &truncated);
      value_copy.bits[3] = 0;  // убираем знак и масштаб
      s21_decimal truncated_fin = truncated;
      s21_mul(truncated, ten, &truncated);
      s21_sub(value_copy, truncated, &ostatok);
      if (is_greater(ostatok, half, 1)) {
        *result = bit_add(truncated_fin, one);
      } else if (is_equal(ostatok, half, 1)) {  // если равно 0.5
        *result = !get_bit(truncated_fin, 0) ? truncated_fin
                                             : bit_add(truncated_fin, one);
      } else {
        *result = truncated_fin;
      }
      if (get_sign(value)) {
        set_sign(result, 1);
      }
    }
    decision = SUCCESS;
  }
  return decision;
}
/*
value = 45
truncated = 40
ostatok = 5
truncated_fin = 4


*/