#include <gtest/gtest.h>

#include "s21_model.h"

TEST(Calculation, Test1) {
  S21::Model model;
  model.Solve("-(+5+0)-(4*8/2^2)mod2+5.5");
  double result = std::stod(model.GetExpression());
  double expected = -(5 + 0) - fmod(4 * 8 / pow(2, 2), 2.0) + 5.5;
  ASSERT_FLOAT_EQ(result, expected);
}

TEST(Calculation, Test2) {
  S21::Model model;
  model.Solve(
      "cos(sin(0.5))+tan(0.5)+acos(0.5)+asin(0.5)+atan(0.5)+"
      "sqrt(0.5)+ln(0.5)+log(0.5)");
  double result = std::stod(model.GetExpression());
  double expected = cos(sin(0.5)) + tan(0.5) + acos(0.5) + asin(0.5) +
                    atan(0.5) + sqrt(0.5) + log(0.5) + log10(0.5);
  ASSERT_FLOAT_EQ(result, expected);
}

TEST(Calculation, Test3) {
  S21::Model model;
  model.Solve("+6-7*(+4)+(-1+3)");
  double result = std::stod(model.GetExpression());
  double expected = 6 - 7 * 4 + (-1 + 3);
  ASSERT_FLOAT_EQ(result, expected);
}

TEST(WrongInput, Test1) {
  S21::Model model;
  model.Solve("(()");
  std::string result = model.GetExpression();
  ASSERT_EQ(result, "ERROR, please provide correct input");
}

TEST(WrongInput, Test2) {
  S21::Model model;
  model.Solve("(())");
  std::string result = model.GetExpression();
  ASSERT_EQ(result, "ERROR, please provide correct input");
}

TEST(WrongInput, Test3) {
  S21::Model model;
  model.Solve("(.)/(.)");
  std::string result = model.GetExpression();
  ASSERT_EQ(result, "ERROR, please provide correct input");
}

TEST(WrongInput, Test4) {
  S21::Model model;
  model.Solve("2.5.5+3");
  std::string result = model.GetExpression();
  ASSERT_EQ(result, "ERROR, please provide correct input");
}

TEST(WrongInput, Test5) {
  S21::Model model;
  model.Solve("5**5");
  std::string result = model.GetExpression();
  ASSERT_EQ(result, "ERROR, please provide correct input");
}

TEST(WrongInput, Test6) {
  S21::Model model;
  model.Solve("4*3(1)");
  std::string result = model.GetExpression();
  ASSERT_EQ(result, "ERROR, please provide correct input");
}

TEST(WrongInput, Test7) {
  S21::Model model;
  model.Solve("*");
  std::string result = model.GetExpression();
  ASSERT_EQ(result, "ERROR, please provide correct input");
}

TEST(WrongInput, Test8) {
  S21::Model model;
  model.Solve(".");
  std::string result = model.GetExpression();
  ASSERT_EQ(result, "ERROR, please provide correct input");
}

TEST(WrongInput, Test9) {
  S21::Model model;
  model.Solve("(5*)");
  std::string result = model.GetExpression();
  ASSERT_EQ(result, "ERROR, please provide correct input");
}

TEST(WrongInput, Tes10) {
  S21::Model model;
  model.Solve("!*4");
  std::string result = model.GetExpression();
  ASSERT_EQ(result, "ERROR, please provide correct input");
}

TEST(WrongInput, Test11) {
  S21::Model model;
  model.Solve("sin(1)4");
  std::string result = model.GetExpression();
  ASSERT_EQ(result, "ERROR, please provide correct input");
}

TEST(WrongInput, Test12) {
  S21::Model model;
  model.Solve("))((");
  std::string result = model.GetExpression();
  ASSERT_EQ(result, "ERROR, please provide correct input");
}

TEST(Credit, Ann) {
  S21::Model model;
  model.credit_vals = {0};
  model.credit_vals.type = model.s21_ANNUITY;
  model.credit_vals.total_credit = 123123.0;
  model.credit_vals.term = 11;
  model.credit_vals.rate = 11;
  model.CreditCalc();
  float exp_montly = 11817.976;
  float exp_overpay = 6874.7339;
  float exp_total_payment = 129997.73;
  ASSERT_FLOAT_EQ(model.credit_vals.monthly, exp_montly);
  ASSERT_FLOAT_EQ(model.credit_vals.overpay, exp_overpay);
  ASSERT_FLOAT_EQ(model.credit_vals.total_payment, exp_total_payment);
}

TEST(Credit, Diff) {
  S21::Model model;
  model.credit_vals = {0};
  model.credit_vals.type = model.s21_DIFFER;
  model.credit_vals.total_credit = 123123.0;
  model.credit_vals.term = 2;
  model.credit_vals.rate = 11;
  model.credit_vals.monthly_interest_p.resize(2);
  model.CreditCalc();
  float exp_montly = 61561.5;
  float exp_overpay = 1692.9413;
  float exp_total_payment = 124815.94;
  double exp_arr[2] = {62690.13, 62125.81};
  ASSERT_FLOAT_EQ(model.credit_vals.monthly_interest_p[0], exp_arr[0]);
  ASSERT_FLOAT_EQ(model.credit_vals.monthly_interest_p[1], exp_arr[1]);
  ASSERT_FLOAT_EQ(model.credit_vals.monthly, exp_montly);
  ASSERT_FLOAT_EQ(model.credit_vals.overpay, exp_overpay);
  ASSERT_FLOAT_EQ(model.credit_vals.total_payment, exp_total_payment);
}

TEST(Deposit, Test1) {
  S21::Model model;
  model.deposit_vals = {0};
  model.deposit_vals.amount = 69420;
  model.deposit_vals.term = 12;
  model.deposit_vals.interest_rate = 5;
  model.deposit_vals.tax_rate = 5;
  model.deposit_vals.periodicity = 12;
  double rep_sum = 489;
  double withd_sum = 69;
  model.DepositCalc(rep_sum, withd_sum);
  float exp_acc = 3471;
  float exp_tax = 173.55;
  float exp_dep = 73311;
  ASSERT_FLOAT_EQ(model.deposit_vals.accrued_interest, exp_acc);
  ASSERT_FLOAT_EQ(model.deposit_vals.tax_amount, exp_tax);
  ASSERT_FLOAT_EQ(model.deposit_vals.deposit_amount, exp_dep);
}

TEST(Graph, Test1) {
  S21::Model model;
  std::vector<double> x_data;
  std::vector<double> y_data;
  model.GetGraphData("x", -1, 1, -1, 1, x_data, y_data);
  ASSERT_EQ(y_data.size(), x_data.size());
  double step = 0.01;
  for (int i = 0; i < x_data.size(); ++i) {
    double x = -1 + step * i;
    ASSERT_FLOAT_EQ(x, x_data[i]);
    ASSERT_FLOAT_EQ(x, y_data[i]);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  //::testing::GTEST_FLAG(filter) = "Calculation*";
  return RUN_ALL_TESTS();
}