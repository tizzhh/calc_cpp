#include "s21_model.h"

void S21::Model::Solve(std::string &input_str) {
  if (!NormalizeString(input_str)) {
    this->expression_ = "ERROR, please provide correct input";
  } else if (!ConvertToPostfix()) {
    this->expression_ = "ERROR, please provide correct input";
  } else {
    Calculate();
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(7) << this->result_.top();
    this->expression_ = oss.str();
  }
}

void S21::Model::Calculate() { return; }

bool S21::Model::ConvertToPostfix() { return true; }

bool S21::Model::NormalizeString(
    std::string &str) {  // чеки, конверт в унарный, замена функций на
                         // соответсвующие значения из енама
  bool check = true;

  if (check) {
    this->expression_ = str;
  }

  return check;
}

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