#include "s21_model.h"

void S21::Model::Solve(std::string &input_str) {
  if (!NormalizeString(input_str)) {
    expression_ = "ERROR, please provide correct input";
  } else if (!ConvertToPostfix()) {
    expression_ = "ERROR, please provide correct input";
  } else {
    Calculate();
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(7) << result_.top();
    expression_ = oss.str();
  }
}

void S21::Model::Calculate() { return; }

bool S21::Model::ConvertToPostfix() { return true; }

bool S21::Model::NormalizeString(
    std::string &str) {  // чеки, конверт в унарный, замена функций на
                         // соответсвующие значения из енама
  bool check = true;
  expression_ = str;

  ReplaceFuncsNames();

  return check;
}

bool S21::Model::CheckParenthesisCorrectness() {
  int left = 0, right = 0;
  bool check = true;
  for (int i = 0, len = expression_.length(); i < len && check; ++i) {
    if (expression_[i] == ')') {
      if (i + 1 < len && !IsOper(expression_[i + 1]) &&
          expression_[i + 1] != ')') {
        check = false;
      }
      if (i >= 1 && IsOper(expression_[i - 1]) && expression_[i - 1] != ')') {
        check = false;
      }
      ++right;
    }

    if (expression_[i] == '(' && check) {
      if (i + 1 < len && expression_[i + 1] == ')') {
        check = false;
      }
      if (i + 1 < len && expression_[i + 1] != '-' &&
          expression_[i + 1] != '+' && !IsFunc(expression_[i + 1]) &&
          !IsDigit(expression_[i + 1]) && expression_[i + 1] != '(') {
        check = false;
      }
      ++left;
    }
  }
  return check && (right == left);
}

bool S21::Model::DotCheck() {
  bool check = true;
  int dot_count = 0;
  if (expression_[0] == '.' || expression_[expression_.length() - 1] == '.') {
    check = false;
  }
  for (int i = 0, len = expression_.length(); i < len && check; ++i) {
    if (expression_[i] == '.') {
      ++dot_count;
    } else if (!IsDigit(expression_[i])) {
      dot_count = 0;
    }
    if (dot_count > 1) {
      check = false;
    }
  }
  return check;
}

bool S21::Model::OperCheck() {
  bool check = true;
  if (IsOper(expression_[0] || IsOper(expression_[expression_.length() - 1]))) {
    check = false;
  }
  for (int i = 0, len = expression_.length(); i < len && check; ++i) {
    if (IsOper(expression_[i])) {
      if (i >= 1 && !IsDigit(expression_[i - 1]) && expression_[i - 1] != ')' &&
          expression_[i] != '-' && expression_[i] != '+') {
        check = false;
      }
      if (i + 1 < len && !IsDigit(expression_[i + 1]) &&
          expression_[i + 1] != '(' && !IsFunc(expression_[i + 1])) {
        check = false;
      }
    }
  }
  return check;
}

bool S21::Model::NumberCheck() {
  bool check = true;
  for (int i = 0, len = expression_.length(); i < len && check; ++i) {
    if (IsDigit(expression_[i])) {
      if (i + 1 < len &&
          !IsOper(expression_[i + 1] && expression_[i + 1] != '.' &&
                  !IsDigit(expression_[i + 1]) && expression_[i + 1] != ')')) {
        check = false;
      }
    }
  }
  return check;
}

void S21::Model::ReplaceFuncsNames() {}

bool S21::Model::IsFunc(const char &ch) {
  return ch >= s21_COS && ch <= s21_LOG;
}

bool S21::Model::IsOper(const char &ch) {
  std::string opers = "+-*/^~`%";
  return opers.find(ch) != std::string::npos;
}

bool S21::Model::IsDigit(const char &ch) { return ch >= '0' && ch <= '9'; }

int main(int argc, char *argv[]) {
  S21::Model m;
  std::string input_string(argv[1]);
  bool isValid = m.NormalizeString(input_string);

  if (isValid) {
    std::cout << "Input string is valid." << std::endl;
  } else {
    std::cout << "Input string is not valid." << std::endl;
  }

  return 0;
}