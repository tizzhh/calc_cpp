#include <check.h>
#include <math.h>

#include "s21_calc.h"

#define s21_PRECISION 1e-7
#define s21_PRECISION_extra 1e-2

START_TEST(test_calc_correct_1) {
  char input_str[] = "-(+5+0)-(4*8/2^2)mod2+5.5";
  char output_str[strlen(input_str) * 2];
  s21_Solve(input_str, output_str);
  double res = atof(output_str);
  double res_pm = -(5 + 0) - fmod(4 * 8 / pow(2, 2), 2.0) + 5.5;
  ck_assert_double_eq_tol(res, res_pm, s21_PRECISION);
}

START_TEST(test_calc_correct_2) {
  char input_str[] =
      "cos(sin(0.5))+tan(0.5)+acos(0.5)+asin(0.5)+atan(0.5)+"
      "sqrt(0.5)+ln(0.5)+log(0.5)";
  char output_str[strlen(input_str) * 2];
  s21_Solve(input_str, output_str);
  double res = atof(output_str);
  double res_pm = cos(sin(0.5)) + tan(0.5) + acos(0.5) + asin(0.5) + atan(0.5) +
                  sqrt(0.5) + log(0.5) + log10(0.5);
  ck_assert_double_eq_tol(res, res_pm, s21_PRECISION);
}

START_TEST(test_calc_correct_3) {
  char input_str[] = "+6-7*(+4)+(-1+3)";
  char output_str[strlen(input_str) * 2];
  s21_Solve(input_str, output_str);
  double res = atof(output_str);
  double res_pm = 6 - 7 * 4 + (-1 + 3);
  ck_assert_double_eq_tol(res, res_pm, s21_PRECISION);
}

START_TEST(test_calc_correct_4) {
  char input_str[] = "x+5";
  s21_Stack *postfix = s21_ConvertToPostfix(input_str);
  s21_push(&postfix, 5, 'x', s21_NUM);
  double res = s21_Calculate(&postfix);
  double res_pm = 5 + 5;
  ck_assert_double_eq_tol(res, res_pm, s21_PRECISION);
  s21_destroy(postfix);
}

START_TEST(test_calc_incorrect_1) {
  char input_str[] = "(()";
  char output_str[strlen(input_str) * 2];
  s21_Solve(input_str, output_str);
  char err[] = "ERROR, please provide correct input";
  ck_assert_str_eq(output_str, err);
}

START_TEST(test_calc_incorrect_2) {
  char input_str[] = "(())";
  char output_str[strlen(input_str) * 2];
  s21_Solve(input_str, output_str);
  char err[] = "ERROR, please provide correct input";
  ck_assert_str_eq(output_str, err);
}

START_TEST(test_calc_incorrect_3) {
  char input_str[] = "(.)/(.)";
  char output_str[strlen(input_str) * 2];
  s21_Solve(input_str, output_str);
  char err[] = "ERROR, please provide correct input";
  ck_assert_str_eq(output_str, err);
}

START_TEST(test_calc_incorrect_4) {
  char input_str[] = "2.5.5+3";
  char output_str[strlen(input_str) * 2];
  s21_Solve(input_str, output_str);
  char err[] = "ERROR, please provide correct input";
  ck_assert_str_eq(output_str, err);
}

START_TEST(test_calc_incorrect_5) {
  char input_str[] = "5**5";
  char output_str[strlen(input_str) * 2];
  s21_Solve(input_str, output_str);
  char err[] = "ERROR, please provide correct input";
  ck_assert_str_eq(output_str, err);
}

START_TEST(test_calc_incorrect_6) {
  char input_str[] = "4*3(1)";
  char output_str[strlen(input_str) * 2];
  s21_Solve(input_str, output_str);
  char err[] = "ERROR, please provide correct input";
  ck_assert_str_eq(output_str, err);
}

START_TEST(test_calc_incorrect_7) {
  char input_str[] = "*";
  char output_str[strlen(input_str) * 2];
  s21_Solve(input_str, output_str);
  char err[] = "ERROR, please provide correct input";
  ck_assert_str_eq(output_str, err);
}

START_TEST(test_calc_incorrect_8) {
  char input_str[] = ".";
  char output_str[strlen(input_str) * 2];
  s21_Solve(input_str, output_str);
  char err[] = "ERROR, please provide correct input";
  ck_assert_str_eq(output_str, err);
}

START_TEST(test_calc_incorrect_9) {
  char input_str[] = "(5*)";
  char output_str[strlen(input_str) * 2];
  s21_Solve(input_str, output_str);
  char err[] = "ERROR, please provide correct input";
  ck_assert_str_eq(output_str, err);
}

START_TEST(test_calc_incorrect_10) {
  char input_str[] = "!*4";
  char output_str[strlen(input_str) * 2];
  s21_Solve(input_str, output_str);
  char err[] = "ERROR, please provide correct input";
  ck_assert_str_eq(output_str, err);
}

START_TEST(test_calc_incorrect_11) {
  char input_str[] = "sin(1)4";
  char output_str[strlen(input_str) * 2];
  s21_Solve(input_str, output_str);
  char err[] = "ERROR, please provide correct input";
  ck_assert_str_eq(output_str, err);
}

START_TEST(test_calc_incorrect_12) {
  char input_str[] = "))((";
  char output_str[strlen(input_str) * 2];
  s21_Solve(input_str, output_str);
  char err[] = "ERROR, please provide correct input";
  ck_assert_str_eq(output_str, err);
}

START_TEST(test_credit_ann) {
  s21_Credit vals = {0};
  vals.total_credit = 123123.0;
  vals.term = 11;
  vals.rate = 11;
  double data[vals.term];
  s21_CreditCalc(&vals, data);
  s21_Credit res = {0};
  res.monthly = 11817.98;
  res.overpay = 6874.73;
  res.total_payment = 129997.73;
  ck_assert_double_eq_tol(vals.monthly, res.monthly, s21_PRECISION_extra);
  ck_assert_double_eq_tol(vals.overpay, res.overpay, s21_PRECISION_extra);
  ck_assert_double_eq_tol(vals.total_payment, res.total_payment,
                          s21_PRECISION_extra);
}

START_TEST(test_credit_diff) {
  s21_Credit vals = {0};
  vals.total_credit = 123123.0;
  vals.term = 2;
  vals.rate = 11;
  vals.type = s21_DIFFER;
  double data[vals.term];
  s21_CreditCalc(&vals, data);
  s21_Credit res = {0};
  res.monthly = 61561.5;
  res.overpay = 1692.94;
  res.total_payment = 124815.94;
  double res_arr[2] = {62690.13, 62125.81};
  ck_assert_double_eq_tol(vals.monthly, res.monthly, s21_PRECISION_extra);
  ck_assert_double_eq_tol(vals.overpay, res.overpay, s21_PRECISION_extra);
  ck_assert_double_eq_tol(vals.total_payment, res.total_payment,
                          s21_PRECISION_extra);
  ck_assert_double_eq_tol(data[0], res_arr[0], s21_PRECISION_extra);
  ck_assert_double_eq_tol(data[1], res_arr[1], s21_PRECISION_extra);
}

START_TEST(test_deposit) {
  s21_Deposit vals = {0};
  vals.amount = 69420;
  vals.term = 12;
  vals.interest_rate = 5;
  vals.tax_rate = 5;
  vals.periodicity = 12;
  double rep_sum = 489;
  double withd_sum = 69;
  s21_DepositCalc(&vals, rep_sum, withd_sum);
  s21_Deposit res = {0};
  res.accrued_interest = 3471;
  res.tax_amount = 173.55;
  res.deposit_amount = 73311;
  ck_assert_double_eq_tol(vals.accrued_interest, res.accrued_interest,
                          s21_PRECISION_extra);
  ck_assert_double_eq_tol(vals.tax_amount, res.tax_amount, s21_PRECISION_extra);
  ck_assert_double_eq_tol(vals.deposit_amount, res.deposit_amount,
                          s21_PRECISION_extra);
}

int main(void) {
  Suite *s = suite_create("Core");
  SRunner *runner = srunner_create(s);
  TCase *tc_core = tcase_create("Core");

  int no_failed = 0;
  tcase_add_test(tc_core, test_calc_correct_1);
  tcase_add_test(tc_core, test_calc_correct_2);
  tcase_add_test(tc_core, test_calc_correct_3);
  tcase_add_test(tc_core, test_calc_correct_4);
  tcase_add_test(tc_core, test_calc_incorrect_1);
  tcase_add_test(tc_core, test_calc_incorrect_2);
  tcase_add_test(tc_core, test_calc_incorrect_3);
  tcase_add_test(tc_core, test_calc_incorrect_4);
  tcase_add_test(tc_core, test_calc_incorrect_5);
  tcase_add_test(tc_core, test_calc_incorrect_6);
  tcase_add_test(tc_core, test_calc_incorrect_7);
  tcase_add_test(tc_core, test_calc_incorrect_8);
  tcase_add_test(tc_core, test_calc_incorrect_9);
  tcase_add_test(tc_core, test_calc_incorrect_10);
  tcase_add_test(tc_core, test_calc_incorrect_11);
  tcase_add_test(tc_core, test_calc_incorrect_12);
  tcase_add_test(tc_core, test_credit_ann);
  tcase_add_test(tc_core, test_credit_diff);
  tcase_add_test(tc_core, test_deposit);
  suite_add_tcase(s, tc_core);
  srunner_run_all(runner, CK_NORMAL);
  no_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (no_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}