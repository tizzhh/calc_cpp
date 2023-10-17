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
  void ConvertUnary();
  bool IsDigit(const char &ch);
  bool IsOper(const char &ch);
  bool IsFunc(const char &ch);
  void ReplaceFuncsNames();
  void GetNum(int &i);
  void HandleOper(const char &oper);
  int CheckPrecedence(const char &oper);
  void HandleRightParenthesis();

  void PrintList();

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
  std::string GetExpression() const noexcept;
};
}  // namespace S21
#endif