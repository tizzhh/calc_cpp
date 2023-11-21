#include "s21_model.h"

void S21::Model::Solve(const std::string &input_str) {
  if (!NormalizeString(input_str)) {
    expression_ = "ERROR, please provide correct input";
  } else {
    ConvertToPostfix();
    double res = Calculate();
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(7) << res;
    expression_ = oss.str();
  }
}

double S21::Model::Calculate() {
  double value = 0, x = 0;
  stack_.clear();
  if (std::get<1>(output_.back()) == 'x') {
    x = std::get<0>(output_.back());
    output_.pop_back();
  }

  auto iter = output_.begin();

  while (iter != output_.end()) {
    if (std::get<2>(*iter) == s21_NUM) {
      if (std::get<1>(*iter) == 'x') {
        stack_.push_back(std::make_tuple(x, 0, s21_NUM));
      } else {
        stack_.push_back(std::make_tuple(std::get<0>(*iter), 0, s21_NUM));
      }
    } else {
      value = std::get<0>(stack_.back());
      stack_.pop_back();
      bool is_binary = IsOperBinary(std::get<1>(*iter));
      HandleOperCalc(value, std::get<0>(stack_.back()), std::get<1>(*iter));
      if (is_binary && !stack_.empty()) {
        stack_.pop_back();
      }
      stack_.push_back(std::make_tuple(value, 0, s21_NUM));
    }
    ++iter;
  }
  value = std::get<0>(stack_.back());
  return value;
}

void S21::Model::HandleOperCalc(double &value, const double &stack_val,
                                const char &oper) {
  if (oper == '+') {
    value += stack_val;
  } else if (oper == '-') {
    value = stack_val - value;
  } else if (oper == '*') {
    value *= stack_val;
  } else if (oper == '/') {
    value = stack_val / value;
  } else if (oper == '^') {
    value = pow(stack_val, value);
  } else if (oper == '%') {
    value = fmod(stack_val, value);
  } else if (oper == '~') {
    value = -value;
  } else if (oper == s21_COS) {
    value = cos(value);
  } else if (oper == s21_SIN) {
    value = sin(value);
  } else if (oper == s21_TAN) {
    value = tan(value);
  } else if (oper == s21_ACOS) {
    value = acos(value);
  } else if (oper == s21_ASIN) {
    value = asin(value);
  } else if (oper == s21_ATAN) {
    value = atan(value);
  } else if (oper == s21_LN) {
    value = log(value);
  } else if (oper == s21_LOG) {
    value = log10(value);
  } else if (oper == s21_SQRT) {
    value = sqrt(value);
  }
}

bool S21::Model::IsOperBinary(const char &oper) {
  std::string opers = "+-*/^%";
  return opers.find(oper) != std::string::npos;
}

void S21::Model::ConvertToPostfix() {  // добавить ошибки, че-то там если
                                       // осталась скобовчка в стэке (в задании
                                       // глянуть)
  output_.clear();
  stack_.clear();
  for (int i = 0, len = expression_.length(); i < len; ++i) {
    if (IsDigit(expression_[i])) {
      GetNum(i);
    } else if (expression_[i] == '(') {
      stack_.push_back(std::make_tuple(0, '(', s21_OPER));
    } else if (IsFunc(expression_[i])) {
      stack_.push_back(std::make_tuple(0, expression_[i], s21_OPER));
    } else if (IsOper(expression_[i])) {
      HandleOper(expression_[i]);
    } else if (expression_[i] == ')') {
      HandleRightParenthesis();
    } else if (expression_[i] == 'x') {
      output_.push_back(std::make_tuple(0, 'x', s21_NUM));
    }
  }
  while (!stack_.empty()) {
    output_.push_back(std::make_tuple(0, std::get<1>(stack_.back()), s21_OPER));
    stack_.pop_back();
  }
}

void S21::Model::PrintList() const noexcept {
  for (const auto &elem : output_) {
    if (std::get<2>(elem) == s21_OPER) {
      std::cout << std::get<1>(elem) << ' ';
    } else {
      std::cout << std::get<0>(elem) << ' ';
    }
  }
  std::cout << '\n';
}

void S21::Model::HandleRightParenthesis() {
  while (!stack_.empty() && std::get<1>(stack_.back()) != '(') {
    output_.push_back(std::make_tuple(0, std::get<1>(stack_.back()), s21_OPER));
    stack_.pop_back();
  }
  if (!stack_.empty()) stack_.pop_back();
  if (!stack_.empty() && IsFunc(std::get<1>(stack_.back()))) {
    output_.push_back(std::make_tuple(0, std::get<1>(stack_.back()), s21_OPER));
    stack_.pop_back();
  }
}

void S21::Model::HandleOper(const char &oper) {
  if (stack_.empty() || oper == '^') {
    stack_.push_back(std::make_tuple(0, oper, s21_OPER));
  } else {
    while (!stack_.empty() && CheckPrecedence(oper) <=
                                  CheckPrecedence(std::get<1>(stack_.back()))) {
      output_.push_back(
          std::make_tuple(0, std::get<1>(stack_.back()), s21_OPER));
      stack_.pop_back();
    }
    stack_.push_back(std::make_tuple(0, oper, s21_OPER));
  }
}

void S21::Model::GetNum(int &i) {
  std::string temp;
  while (IsDigit(expression_[i]) || expression_[i] == '.') {
    temp += expression_[i];
    ++i;
  }
  --i;
  output_.push_back(std::make_tuple(std::stod(temp), 0, s21_NUM));
}

int S21::Model::CheckPrecedence(const char &oper) {
  int precedence = 0;
  if ((oper >= s21_COS && oper <= s21_LOG) || oper == '%') {
    precedence = 4;
  } else if (oper == '~' || oper == '`') {
    precedence = 3;
  } else if (oper == '*' || oper == '/') {
    precedence = 2;
  } else if (oper == '-' || oper == '+') {
    precedence = 1;
  }
  return precedence;
}

bool S21::Model::NormalizeString(const std::string &str) {
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

void S21::Model::CreditCalc() {
  this->credit_vals.rate /= 100.0;
  double monthly_rate = this->credit_vals.rate / 12.0;
  if (this->credit_vals.type == s21_ANNUITY) {
    this->credit_vals.monthly =
        this->credit_vals.total_credit *
        ((monthly_rate * pow(1 + monthly_rate, this->credit_vals.term)) /
         (pow(1 + monthly_rate, this->credit_vals.term) - 1));
    this->credit_vals.total_payment =
        this->credit_vals.monthly * this->credit_vals.term;
    this->credit_vals.overpay =
        (this->credit_vals.monthly * this->credit_vals.term) -
        this->credit_vals.total_credit;
  } else if (this->credit_vals.type == s21_DIFFER) {
    double montly_total_credit =
        this->credit_vals.total_credit / this->credit_vals.term;
    for (int i = 0; i < this->credit_vals.term; ++i) {
      this->credit_vals.monthly =
          this->credit_vals.total_credit / this->credit_vals.term;
      this->credit_vals.monthly_interest_p[i] =
          (this->credit_vals.total_credit -
           (montly_total_credit * (i + 1 - 1))) *
          monthly_rate;
      this->credit_vals.monthly_interest_p[i] += montly_total_credit;
      this->credit_vals.total_payment +=
          this->credit_vals.monthly_interest_p[i];
    }
    this->credit_vals.overpay =
        this->credit_vals.total_payment - this->credit_vals.total_credit;
  }
}

void S21::Model::DepositCalc(double rep_summ, double withd_summ) {
  this->deposit_vals.tax_rate /= 100;
  this->deposit_vals.interest_rate /= 100;
  for (int i = 0; i < this->deposit_vals.term; ++i) {
    double cum_amount =
        this->deposit_vals.amount * this->deposit_vals.interest_rate /
        this->deposit_vals
            .periodicity;  // надо починить периодичность (при периодичности 12
                           // - норм, при другой - нет)
    this->deposit_vals.accrued_interest += cum_amount;
    this->deposit_vals.amount += cum_amount * this->deposit_vals.capitalization;
  }

  this->deposit_vals.tax_amount =
      this->deposit_vals.tax_rate * this->deposit_vals.accrued_interest;

  this->deposit_vals.deposit_amount =
      this->deposit_vals.amount + rep_summ - withd_summ +
      (this->deposit_vals.capitalization ? 0
                                         : this->deposit_vals.accrued_interest);
}

void S21::Model::GetGraphData(const std::string &input_str, double x_min,
                              double x_max, double y_min, double y_max,
                              std::vector<double> &x_data,
                              std::vector<double> &y_data) {
  double step = 0.01;
  std::string output = input_str;
  expression_ = output;
  ConvertUnary();
  ConvertToPostfix();
  for (int i = 0; i <= (x_max - x_min) / step; ++i) {
    double y = 0.0;
    double x = x_min + i * step;
    output_.push_back(std::make_tuple(x, 'x', s21_NUM));
    y = Calculate();
    x_data.push_back(x);
    y_data.push_back(y);
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

// int main(int argc, char *argv[]) {
//   if (argc != 2) {
//     return 1;
//   }
//   S21::Model m;
//   std::string input_string(argv[1]);

//   m.Solve(input_string);
//   std::cout << m.GetExpression() << '\n';

//   // m.NormalizeString(input_string);
//   // m.ConvertToPostfix();

//   // bool isValid = m.NormalizeString(input_string);
//   // if (isValid) {
//   //   std::cout << "Input string is valid." << std::endl;
//   //   std::cout << '\n' << m.GetExpression() << '\n';
//   // } else {
//   //   std::cout << "ERROR, please provide correct input" << std::endl;
//   // }
// }
