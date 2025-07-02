#include "s21_decimal.h"

void printdec(s21_decimal num) {
  for (int i = 95; i >= 0; i--) {
    printf("%d", get_bit(num, i));
  }
  printf("\n");
}

int main() {
  int test = 0;
  scanf("%d", &test);
  s21_decimal num1 = {{3, 0, 0, 0}, NORMAL};
  s21_decimal num2 = {{0, 0, 0, 0}, NORMAL};
  set_scale(&num1, 0);
  set_scale(&num2, 0);
  set_sign(&num1, 0);
  set_sign(&num2, 0);
  s21_decimal res = {0};
  int dec = 0;
  switch (test) {
    case 1:  // деление
      dec = s21_div(num1, num2, &res);
      printdec(res);
      printf("decision = %d\nvalue = %d\nsign = %d\nscale = %d\n", dec,
             res.value_condition, get_sign(res), get_scale(&res));
      break;
    case 2:  // сложение
      dec = s21_add(num1, num2, &res);
      printdec(res);
      printf("value = %d\nsign = %d\nscale = %d\n", dec, get_sign(res),
             get_scale(&res));
      break;
    case 3:  // вычитание
      dec = s21_sub(num1, num2, &res);
      printdec(res);
      printf("value = %d\nsign = %d\nscale = %d\n", dec, get_sign(res),
             get_scale(&res));
      break;
    case 4:  // умножение
      dec = s21_mul(num1, num2, &res);
      printdec(res);
      printf("decision = %d\nvalue = %d\nsign = %d\nscale = %d\n", dec,
             res.value_condition, get_sign(res), get_scale(&res));
      break;
    case 5:  // is greater
      printf("result = %d\n", is_greater(num1, num2, 0));
      break;
    case 6:  // is less
      printf("result = %d\n", s21_is_less_or_equal(num1, num2));
      break;
    case 7:  // is equal
      printf("result = %d\n", is_equal(num1, num2, 0));
      break;
    case 8:  // scale_equalize
      scale_equalize(&num1, &num2);
      printf("scale1 = %d\nscale2 = %d\n", get_scale(&num1), get_scale(&num2));
      break;
    case 9:  // 10mul, не поддерживает больше 19
      res = bit_10mul(num1, 19);
      printdec(res);
      break;
    case 10:  // dec to int
      int i = 0;
      dec = s21_from_decimal_to_int(num1, &i);
      printf("converted int = %d and decision = %d\n", i, dec);
      break;
    case 11:  // int to dec
      int srci = 120;
      s21_from_int_to_decimal(srci, &res);
      srci = 0;
      dec = s21_from_decimal_to_int(res, &srci);
      printdec(res);
      printf("int = %d\ndecision = %d\nsign = %d\n", srci, dec, get_sign(res));
      break;
    case 12:  // float to dec
      float srcf = 1200.0;
      dec = s21_from_float_to_decimal(srcf, &res);
      printdec(res);
      printf("decision = %d\nsign = %d\nscale = %d\n", dec, get_sign(res),
             get_scale(&res));
      printf("%f\n", trunc(1.333 * 10 * 10));
      break;
    case 13:  // dec to float
      float srcf2 = 0.0;
      dec = s21_from_decimal_to_float(num1, &srcf2);
      printf("%f\n", srcf2);
      break;
    case 14:  // floor
      s21_decimal floor_res = {0};
      dec = s21_floor(num1, &floor_res);
      printdec(floor_res);
      printf("sign = %d\nscale = %d\ndecision = %d\ncondition = %d\n",
             get_sign(floor_res), get_scale(&floor_res), dec,
             floor_res.value_condition);
      break;
    case 15:  // round
      s21_decimal round_res = {0};
      dec = s21_round(num1, &round_res);
      printdec(round_res);
      printf("sign = %d\nscale = %d\ndecision = %d\ncondition = %d\n",
             get_sign(round_res), get_scale(&round_res), dec,
             round_res.value_condition);
      break;
    case 16:  // truncate
      s21_decimal truncate_res = {0};
      dec = s21_truncate(num1, &truncate_res);
      printdec(truncate_res);
      printf("sign = %d\nscale = %d\ndecision = %d\ncondition = %d\n",
             get_sign(truncate_res), get_scale(&truncate_res), dec,
             truncate_res.value_condition);
      break;
    case 17:  // print num1
      printdec(num1);
      printf("sign = %d\nscale = %d\n", get_sign(num1), get_scale(&num1));
      break;
    case 18:  // print num2
      printdec(num2);
      printf("sign = %d\nscale = %d\n", get_sign(num2), get_scale(&num2));
      break;
    case 19:  // bit div
      s21_decimal musor = {0};
      s21_decimal div_res = bit_div(num1, num2, &musor);
      printdec(div_res);
      printdec(musor);
      break;
    case 20:  // is not equal
      scale_equalize(&num1, &num2);
      printf("%d and %d", get_sign(num1), get_sign(num2));
      break;
  }
  return 0;
}