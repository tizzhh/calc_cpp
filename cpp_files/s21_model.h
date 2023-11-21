#ifndef CPP3_SMART_CALC_V_2_0_CPP_FILES_S21_MODEL_H
#define CPP3_SMART_CALC_V_2_0_CPP_FILES_S21_MODEL_H

#include <cmath>
#include <iomanip>
#include <iostream>
#include <list>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace S21 {
class Model {
 private:
  std::string expression_;
  std::list<std::tuple<double, char, int>> stack_;
  std::list<std::tuple<double, char, int>> output_;
  bool NumberCheck();
  bool OperCheck();
  bool DotCheck();
  bool CheckParenthesisCorrectness();
  bool IsDigit(const char &ch);
  bool IsOper(const char &ch);
  bool IsFunc(const char &ch);
  void ReplaceFuncsNames();
  void GetNum(int &i);
  void HandleOper(const char &oper);
  int CheckPrecedence(const char &oper);
  void HandleRightParenthesis();
  bool IsOperBinary(const char &oper);
  void HandleOperCalc(double &value, const double &stack_val, const char &oper);

  void PrintList() const noexcept;

  enum s21_Funcs_ {
    s21_COS = 'a',
    s21_SIN,
    s21_TAN,
    s21_ACOS,
    s21_ASIN,
    s21_ATAN,
    s21_SQRT,
    s21_LN,
    s21_LOG,
  };

  typedef struct s21_Credit {
    double total_credit;
    int term;
    double rate;
    int type;
    double monthly;
    double overpay;
    double total_payment;
    std::vector<double> monthly_interest_p;
  } s21_Credit;

  typedef struct s21_Deposit {
    double amount;
    int term;
    double interest_rate;
    double tax_rate;
    int periodicity;
    int capitalization;
    double accrued_interest;
    double tax_amount;
    double deposit_amount;
  } s21_Deposit;

 public:
  enum Types {
    s21_NUM,
    s21_OPER,
    s21_ANNUITY,
    s21_DIFFER,
  };

  Model() = default;
  ~Model() = default;

  s21_Credit credit_vals;
  s21_Deposit deposit_vals;

  void Solve(const std::string &input_str);
  bool NormalizeString(const std::string &str);
  void ConvertToPostfix();
  double Calculate();
  std::string GetExpression() const noexcept;
  void CreditCalc();
  void DepositCalc(double rep_summ, double withd_summ);
  void ConvertUnary();
  void GetGraphData(const std::string &input_str, double x_min, double x_max,
                    double y_min, double y_max, std::vector<double> &x_data,
                    std::vector<double> &y_data);
};
}  // namespace S21
#endif