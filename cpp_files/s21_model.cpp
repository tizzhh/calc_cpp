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
  if (!CheckParenthesisCorrectness()) {
    check = false;
  } else if (!DotCheck()) {
    check = false;
  } else if (!OperCheck()) {
    check = false;
  } else if (!NumberCheck()) {
    check = false;
  } else {
    ConvertUnary();
  }
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
  std::string opers_without_unary = "*/^~`%";
  bool unary_check =
      opers_without_unary.find(expression_[0]) != std::string::npos;
  if (unary_check || IsOper(expression_[expression_.length() - 1])) {
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
      if (i + 1 < len && !IsOper(expression_[i + 1]) &&
          expression_[i + 1] != '.' && !IsDigit(expression_[i + 1]) &&
          expression_[i + 1] != ')') {
        check = false;
      }
    }
  }
  return check;
}

void S21::Model::ConvertUnary() {
  if (expression_[0] == '-') {
    expression_[0] = '~';
  } else if (expression_[0] == '+') {
    expression_[0] = '`';
  }
  for (int i = 1, len = expression_.length(); i < len; ++i) {
    if (expression_[i] == '-' && expression_[i - 1] == '(') {
      expression_[i] = '~';
    }
    if (expression_[i] == '+' && expression_[i - 1] == '(') {
      expression_[i] = '`';
    }
  }
}

void S21::Model::ReplaceFuncsNames() {
  std::unordered_map<std::string, char> funcMappings = {
      {"sin", s21_SIN},   {"cos", s21_COS},   {"tan", s21_TAN},
      {"acos", s21_ACOS}, {"asin", s21_ASIN}, {"atan", s21_ATAN},
      {"sqrt", s21_SQRT}, {"ln", s21_LN},     {"log", s21_LOG},
      {"mod", '%'}};

  for (const auto &mapping : funcMappings) {
    size_t pos = 0;
    while ((pos = expression_.find(mapping.first, pos)) != std::string::npos) {
      expression_.replace(pos, mapping.first.length(), 1, mapping.second);
      ++pos;
    }
  }
}

bool S21::Model::IsFunc(const char &ch) {
  return ch >= s21_COS && ch <= s21_LOG;
}

bool S21::Model::IsOper(const char &ch) {
  std::string opers = "+-*/^~`%";
  return opers.find(ch) != std::string::npos;
}

bool S21::Model::IsDigit(const char &ch) { return ch >= '0' && ch <= '9'; }

std::string S21::Model::GetExpression() const noexcept { return expression_; }

int main(int argc, char *argv[]) {
  S21::Model m;
  std::string input_string(argv[1]);
  bool isValid = m.NormalizeString(input_string);

  if (isValid) {
    std::cout << "Input string is valid." << std::endl;
    std::cout << '\n' << m.GetExpression() << '\n';
  } else {
    std::cout << "ERROR, please provide correct input" << std::endl;
  }

  return 0;
}