#ifndef CPP3_SMART_CALC_V_2_0_CPP_FILES_S21_MODEL_H
#define CPP3_SMART_CALC_V_2_0_CPP_FILES_S21_MODEL_H

#include <cmath>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>

namespace S21 {
class Model {
 private:
  std::string expression_;
  std::stack<std::tuple<double, char, int>> postfix_;
  std::stack<double> result_;
  bool NumberCheck();
  bool OperCheck();
  bool DotCheck();
  bool CheckParenthesisCorrectness();
  bool IsDigit(const char &ch);
  bool IsOper(const char &ch);
  bool IsFunc(const char &ch);
  void ReplaceFuncsNames();

  enum s21_funcs {
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

  enum Types {
    s21_NUM,
    s21_OPER,
    s21_ANNUITY,
    s21_DIFFER,
  };

 public:
  Model() = default;
  ~Model() = default;

  void Solve(std::string &input_str);
  bool NormalizeString(std::string &str);
  bool ConvertToPostfix();
  void Calculate();
};
}  // namespace S21
#endif