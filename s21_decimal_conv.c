#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int res = SUCCESS;
  if (dst != NULL) {
    dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = 0;
    if (src < 0) {
      dst->bits[0] = ~src + 1;
      set_sign(dst, 1);
    } else {
      dst->bits[0] = src;
    }
  } else {
    res = FAIL;
  }
  return res;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int res = FAIL;
  if (src.value_condition == NORMAL) {
    *dst = 0;
    s21_decimal copy = src;
    for (int i = get_scale(&src); i > 0; i--) {
      s21_decimal ten = {{10, 0, 0, 0}, NORMAL};
      s21_decimal musor = {0};
      copy = bit_div(copy, ten, &musor);
    }
    if (copy.bits[1] == 0 && copy.bits[2] == 0 && copy.bits[0] <= 2147483647 &&
        copy.bits[0] >= -2147483647) {
      *dst = copy.bits[0];
      if (get_sign(src)) *dst = -*dst;
      res = SUCCESS;
    }
  }
  return res;
}

int get_double_exponent(double num) {
  union {
    double d;
    unsigned long u;
  } data;
  data.d = num;
  return (unsigned char)((data.u >> 52) - 1023);
}

int get_double_bit(double num, int position) {
  union {
    double f;
    unsigned long u;
  } data;
  data.f = num;
  unsigned long mask = 1UL << position;
  return (data.u & mask) != 0;
}

int s21_from_float_to_decimal(
    float src, s21_decimal *dst) {  // подравнять, она пока как инта действует
  int res = FAIL;
  int sign = 0;
  if (src < 0.0) sign = 1;
  if (dst != NULL && !isinf(src)) {
    dst->bits[0] = dst->bits[1] = dst->bits[2] = dst->bits[3] = 0;
    dst->value_condition = NORMAL;
    int scale = 0;
    int flag = 0;
    double tmp = (double)fabs(src);
    while (scale < 28 && (int)tmp / (int)pow(2, 21) == 0 &&
           !flag) {  // 2^21 - максимальное точное число во флоат, ибо
                     // гарантированно не превышает 23 бита мантиссы
      tmp *= 10;
      scale++;
      if (isinf(tmp)) flag = 1;
    }
    if (!flag) {
      tmp = round(tmp);
      for (int i = scale; i > 0 && fmod(tmp, 10) == 0.0;
           i--) {  // если есть скейл, то делит
        tmp /= 10;
        scale--;
      }
      tmp = (float)tmp;
      int exponent = get_double_exponent(tmp);
      set_bit(dst, exponent, 1);  // установка неявной единицы
      for (int i = 51, j = exponent - 1; i >= 0 && j >= 0; i--,
               j--) {  // собсна перевод битов из дабла в децимала, идём либо
                       // пока не кончится дабл, либо пока экпонента не кончится
        set_bit(dst, j, get_double_bit(tmp, i));
      }
      set_scale(dst, scale);
      if (sign) set_sign(dst, 1);
      res = SUCCESS;
    }
  }
  return res;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int result = FAIL;
  if (src.value_condition == NORMAL) {
    double tmp = 0.0;
    for (int i = 0; i < 96; i++) {
      if (get_bit(src, i)) tmp += pow(2, i);
    }
    for (int i = get_scale(&src); i > 0; i--) {
      tmp /= 10;
    }
    *dst = (float)tmp;
    if (get_sign(src)) *dst *= -1;
    result = SUCCESS;
  }
  return result;
}
