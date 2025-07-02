#include "s21_decimal.h"

int get_bit(s21_decimal num, int bit) {
  int res = 0;
  if (bit / 32 < 4) {
    unsigned mask = 1 << (bit % 32);
    res = num.bits[bit / 32] & mask;
  }
  return res != 0;
}

void set_bit(s21_decimal *num, int bit, int val) {
  unsigned mask = 1 << (bit % 32);
  if (bit / 32 < 4) {
    if (val) {
      num->bits[bit / 32] |= mask;
    } else {
      num->bits[bit / 32] &= ~mask;  // ~mask: 0000100 = 1111011  (как пример)
    }
  }
}

int get_sign(s21_decimal num) {
  unsigned mask = 1 << 31;
  return (num.bits[3] & mask) != 0;
}

void set_sign(s21_decimal *num, int sign) {
  unsigned mask = 1 << 31;
  if (sign) {
    num->bits[3] |= mask;
  } else if (!sign) {
    num->bits[3] &= ~mask;
  }
}

int get_scale(const s21_decimal *num) { return (char)(num->bits[3] >> 16); }

void set_scale(s21_decimal *num, int scale) {
  if (scale < 0 || scale > 28) {
    printf("wrong scale");
  } else {
    int clear_scale = ~(0xFF << 16);
    num->bits[3] &= clear_scale;
    int scaling = scale << 16;
    num->bits[3] |= scaling;
  }
}

void offset_left(s21_decimal *num, int offset_num) {
  for (int i = 0; i < offset_num; i++) {
    int first_edge_bit = get_bit(*num, 31);
    int second_edge_bit = get_bit(*num, 63);
    if (get_bit(*num, 95)) {
      num->value_condition = get_sign(*num) ? MINUS_INF : PLUS_INF;
    }
    num->bits[0] <<= 1;
    num->bits[1] <<= 1;
    num->bits[2] <<= 1;
    set_bit(num, 32, first_edge_bit);
    set_bit(num, 64, second_edge_bit);
  }
}

void offset_right(s21_decimal *num, int offset_num) {  // не полнорабочая
  for (int i = 0; i < offset_num; i++) {
    int first_edge_bit = get_bit(*num, 32);
    int second_edge_bit = get_bit(*num, 64);
    num->bits[0] >>= 1;
    num->bits[1] >>= 1;
    num->bits[2] >>= 1;
    set_bit(num, 31, first_edge_bit);
    set_bit(num, 63, second_edge_bit);
  }
}

int find_high_bit(s21_decimal num1) {  // ищет самый старший бит
  int res = 0;
  int flag = 0;
  for (int i = 95; i >= 0 && !flag; i--) {
    if (get_bit(num1, i) == 1) {
      res = i;
      flag = 1;
    }
  }
  return res;
}

s21_decimal bit_add(s21_decimal num1, s21_decimal num2) {  // побитовое сложение
  s21_decimal res = {0};
  int perenos = 0;
  for (int i = 0; i < 96; i++) {
    int first_bit = get_bit(num1, i);
    int second_bit = get_bit(num2, i);
    int summ = first_bit + second_bit;
    if (summ == 2) {  // 1 + 1 = 2 -> 0
      if (perenos) {
        set_bit(&res, i, 1);
        perenos = 1;
      } else {
        set_bit(&res, i, 0);
        perenos = 1;
      }
    } else if (summ == 0) {  // 0 + 0
      if (perenos) {
        set_bit(&res, i, 1);
        perenos = 0;
      } else {
        set_bit(&res, i, 0);
        perenos = 0;
      }
    } else if (summ == 1) {  // 0 + 1 или 1 + 0
      if (perenos) {
        set_bit(&res, i, 0);
        perenos = 1;
      } else {
        set_bit(&res, i, 1);
        perenos = 0;
      }
    }
    if ((i == 95) && (perenos)) res.value_condition = PLUS_INF;
  }
  return res;
}

s21_decimal bit_mul(s21_decimal num1, s21_decimal num2) {  // побитовое
                                                           // умножение
  s21_decimal res = {{0, 0, 0, 0}, NORMAL};
  int flag = 0;
  for (int i = 0; i < 96 && !flag; i++) {
    if (get_bit(num2, i)) {
      s21_decimal shifted = num1;
      offset_left(&shifted, i);
      if (get_bit(shifted, 95) && i > 0) {
        res.value_condition = get_sign(num1) ? MINUS_INF : PLUS_INF;
        flag = 1;
      } else {
        res = bit_add(res, shifted);
      }
    }
  }
  return res;
}

s21_decimal bit_sub(s21_decimal num1, s21_decimal num2) {
  s21_decimal res = {0};
  int ostatok = 0;
  if (is_greater(num1, num2, 1)) {
    for (int i = 0; i < 96; i++) {
      int first_bit = get_bit(num1, i);
      int second_bit = get_bit(num2, i);
      int fin = (first_bit - ostatok) - second_bit;
      if (fin < 0) {
        fin += 2;
        ostatok = 1;
      } else {
        ostatok = 0;
      }
      set_bit(&res, i, fin);
      if (i == 95 && ostatok) res.value_condition = MINUS_INF;
    }
  }
  return res;
}

s21_decimal bit_div(s21_decimal num1, s21_decimal num2,
                    s21_decimal *ostatok) {  // побитовое деление
  s21_decimal res = {0};
  *ostatok = res;
  set_scale(&num1, 0);
  set_scale(&num2, 0);
  for (int i = find_high_bit(num1); i >= 0; i--) {
    set_bit(ostatok, 0, get_bit(num1, i));
    if (is_greater_or_equal(*ostatok, num2, 1)) {
      *ostatok = bit_sub(*ostatok, num2);
      offset_left(&res, 1);
      if (i != 0) offset_left(ostatok, 1);
      set_bit(&res, 0, 1);
    } else {
      if (i != 0) offset_left(ostatok, 1);
      offset_left(&res, 1);
    }
  }
  return res;
}

void calc_diff(s21_decimal num1, s21_decimal num2, s21_decimal *res) {
  if (is_greater(num1, num2, 1)) {
    *res = bit_sub(num1, num2);
    set_sign(res, get_sign(num1));
  } else if (is_less(num1, num2, 1)) {
    *res = bit_sub(num2, num1);
    set_sign(res, get_sign(num2));
  }
}

s21_decimal bit_10mul(s21_decimal num,
                      int scale) {  // функция для умножения числа на 10
  s21_decimal res = num;
  s21_decimal temp1 = {0};
  s21_decimal temp2 = {0};
  for (int i = 0; i < scale; i++) {
    temp1 = res;
    temp2 = res;
    offset_left(&temp1, 1);
    offset_left(&temp2, 3);
    res = bit_add(temp1, temp2);
    set_scale(&num, 0);
  }
  return res;
}

int scale_equalize(s21_decimal *num1, s21_decimal *num2) {
  int fin_scale = 0;
  char scale1 = get_scale(num1);
  char scale2 = get_scale(num2);
  char sign1 = get_sign(*num1);
  char sign2 = get_sign(*num2);

  if (scale1 != scale2) {
    if (scale1 > scale2) {
      *num2 = bit_10mul(*num2, scale1 - scale2);
      set_scale(num2, scale1);
      fin_scale = scale1;
    } else {
      *num1 = bit_10mul(*num1, scale2 - scale1);
      set_scale(num1, scale2);
      fin_scale = scale2;
    }
    set_sign(num1, sign1);
    set_sign(num2, sign2);
  } else {
    fin_scale = scale1;
  }
  return fin_scale;
}

int zero_check(s21_decimal num) {
  int res = 0;
  if (!num.bits[0] && !num.bits[1] && !num.bits[2]) {
    res = 1;
  }
  return res;
}

s21_decimal num_check(s21_decimal num1, s21_decimal num2) {
  s21_decimal res = {0};
  res.value_condition = num1.value_condition > num2.value_condition
                            ? num1.value_condition
                            : num2.value_condition;
  return res;
}