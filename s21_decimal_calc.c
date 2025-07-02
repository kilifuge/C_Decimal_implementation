#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  *result = num_check(value_1, value_2);
  int decision = result->value_condition;
  if (decision == NORMAL) {
    char sign1 = get_sign(value_1);
    char sign2 = get_sign(value_2);
    int fin_scale = scale_equalize(&value_1, &value_2);
    if (value_1.value_condition || value_2.value_condition) {
      decision = sign1 ? MINUS_INF : PLUS_INF;
    } else {
      if (sign1 == sign2) {
        *result = bit_add(value_1, value_2);
        set_sign(result, sign1);
        if (result->value_condition) {
          decision = sign1 ? MINUS_INF : PLUS_INF;
        }
      } else if (sign1 != sign2) {
        calc_diff(value_1, value_2, result);
      }  // а если два числа по модулю равны, то мы просто ничего не делаем,
         // число и так нулевое
      set_scale(result, fin_scale);
    }
  }
  return decision;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  *result = num_check(value_1, value_2);
  int decision = result->value_condition;
  if (decision == NORMAL) {
    char sign1 = get_sign(value_1);
    char sign2 = get_sign(value_2);
    int fin_scale = scale_equalize(&value_1, &value_2);
    if (value_1.value_condition || value_2.value_condition) {
      decision = sign1 ? MINUS_INF : PLUS_INF;
    } else {
      if (sign1 != sign2) {
        *result = bit_add(value_1, value_2);
        set_sign(result, sign1);
        if (result->value_condition) {
          decision = sign1 ? MINUS_INF : PLUS_INF;
        }
      } else {
        if (is_less(value_1, value_2, 1)) set_sign(&value_2, 1);
        calc_diff(value_1, value_2, result);
      }
      set_scale(result, fin_scale);
    }
  }
  return decision;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result) {  // пока плохо отлавливает переполнение
  *result = num_check(value_1, value_2);
  int decision = result->value_condition;
  if (decision == NORMAL) {
    *result = bit_mul(value_1, value_2);
    if (get_sign(value_1) != get_sign(value_2)) set_sign(result, 1);
    if (result->value_condition) {
      decision = get_sign(*result) ? MINUS_INF : PLUS_INF;
    }
    set_scale(result, get_scale(&value_1) + get_scale(&value_2));
  }
  return decision;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  *result = num_check(value_1, value_2);
  s21_decimal zero = {0};
  s21_decimal ten = {{10, 0, 0, 0}, NORMAL};
  int decision = result->value_condition;
  if (is_equal(value_2, zero, 1)) {  // если деление на ноль
    decision = ZERO_SUB;
  } else if (decision == NORMAL) {
    s21_decimal ostatok = {0};
    s21_decimal tmp = {0};
    int start_scale = get_scale(&value_1) - get_scale(&value_2);
    int cycle_scale = 0;
    *result = bit_div(value_1, value_2, &ostatok);
    if (!is_equal(ostatok, zero, 1) &&
        !result->value_condition) {  // т.е если после деления остался остаток.
      for (; cycle_scale < 20 && !is_equal(ostatok, zero, 1);
           cycle_scale++) {  // для примера, 17\3 = 5(5,66666667), остаток 2
        tmp = bit_10mul(ostatok, 1);            // 2 * 10 = 20
        tmp = bit_div(tmp, value_2, &ostatok);  // 20\3 = 6, остаток 2
        *result = bit_10mul(*result, 1);  // 8 * 10 = 80, чтобы записать 6-ку
        *result = bit_add(
            *result,
            tmp);  // 80 + 6 = 86, и так по кругу, пока не получим 566666667
      }
    }
    int fin_scale = start_scale + cycle_scale;
    while (fin_scale < 0) {
      *result = bit_10mul(*result, 1);
      fin_scale++;
    }
    while (fin_scale > 28) {
      s21_decimal no_need = {
          0};  // просто чтобы функция работала, ненужные данные
      *result = bit_div(*result, ten, &no_need);
      fin_scale--;
    }
    set_scale(result, fin_scale);
    if (get_sign(value_1) != get_sign(value_2)) set_sign(result, 1);
  }
  return decision;
}
