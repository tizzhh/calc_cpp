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

 public:
  Model() = default;
  ~Model() = default;

  void Solve(std::string &input_str);
  bool NormalizeString(std::string &str);
  bool ConvertToPostfix();
  void Calculate();

  enum Types {
    s21_NUM,
    s21_OPER,
    s21_ANNUITY,
    s21_DIFFER,
  };
};
}  // namespace S21
#endif