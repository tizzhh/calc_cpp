#include "s21_calc.h"

char *s21_Solve(char *input_str, char *output_str) {
  if (!s21_NormalizeStr(input_str) ||
      !strcmp("ERROR, please provide correct input", input_str)) {
    strcpy(output_str, "ERROR, please provide correct input");
    return output_str;
  }
  s21_Stack *postfix = s21_ConvertToPostfix(input_str);
  double res = s21_Calculate(&postfix);
  s21_destroy(postfix);
  sprintf(output_str, "%.7f", res);
  return output_str;
}

double s21_Calculate(s21_Stack **postfixx) {
  double value = 0.0;
  s21_Stack *result = NULL;
  s21_Stack *postfix = *postfixx;

  double x = 0;
  if (postfix->oper == 'x') {
    x = postfix->val;
    postfix = postfix->next;
    s21_pop(postfixx);
  }

  while (postfix != NULL) {
    if (postfix->type == s21_NUM) {
      if (postfix->oper == 'x') {
        s21_push(&result, x, 0, s21_NUM);
      } else {
        s21_push(&result, postfix->val, 0, s21_NUM);
      }
    } else {
      value = result->val;
      s21_pop(&result);
      int is_binary = s21_CheckOperType(postfix->oper);
      printf("%f\n", value);
      s21_HandleOperCalc(&value, result ? result->val : 0, postfix->oper);
      printf("%f\n", value);
      if (is_binary && result != NULL) {
        s21_pop(&result);
      }
      s21_push(&result, value, 0, s21_NUM);
    }
    postfix = postfix->next;
  }
  value = result->val;
  s21_destroy(result);
  return value;
}

void s21_HandleOperCalc(double *value, double stack_val, char oper) {
  if (oper == '+') {
    *value += stack_val;
  } else if (oper == '-') {
    *value = stack_val - *value;
  } else if (oper == '*') {
    *value *= stack_val;
  } else if (oper == '/') {
    *value = stack_val / *value;
  } else if (oper == '^') {
    *value = pow(stack_val, *value);
  } else if (oper == s21_MOD) {
    *value = fmod(stack_val, *value);
  } else if (oper == '~') {
    *value = -(*value);
  } else if (oper == s21_COS) {
    *value = cos(*value);
  } else if (oper == s21_SIN) {
    *value = sin(*value);
  } else if (oper == s21_TAN) {
    *value = tan(*value);
  } else if (oper == s21_ACOS) {
    *value = acos(*value);
  } else if (oper == s21_ASIN) {
    *value = asin(*value);
  } else if (oper == s21_ATAN) {
    *value = atan(*value);
  } else if (oper == s21_LN) {
    *value = log(*value);
  } else if (oper == s21_LOG) {
    *value = log10(*value);
  } else if (oper == s21_SQRT) {
    *value = sqrt(*value);
  }
}

void s21_ReverseList(s21_Stack **prefix) {
  s21_Stack *prev, *next;
  s21_Stack *curr = *prefix;
  prev = next = NULL;
  while (curr != NULL) {
    next = curr->next;
    curr->next = prev;
    prev = curr;
    curr = next;
  }
  *prefix = prev;
}

int s21_NormalizeStr(char *input_str) {
  int check = 1;
  if (!s21_CheckParenthesisCorrectness(input_str)) {
    check = 0;
  } else if (!s21_DotCheck(input_str)) {
    check = 0;
  } else if (!s21_OperCheck(input_str)) {
    check = 0;
  } else if (!s21_NumberCheck(input_str)) {
    check = 0;
  } else {
    s21_ConvertUnary(input_str);
  }
  return check;
}

int s21_NumberCheck(char *input_str) {
  int check = 1;
  for (int i = 0, len = strlen(input_str); i < len && check; ++i) {
    if (s21_isdigit(input_str[i])) {
      if (i + 1 < len &&
          (!s21_isOper(input_str[i + 1]) && (input_str[i + 1] != '.') &&
           !s21_isdigit(input_str[i + 1]) && (input_str[i + 1] != ')'))) {
        check = 0;
      }
    }
  }
  return check;
}

int s21_OperCheck(char *input_str) {
  int check = 1;
  if (s21_isOperWithoutUnary(input_str[0]) ||
      s21_isOper(input_str[strlen(input_str) - 1])) {
    check = 0;
  }
  for (int i = 0, len = strlen(input_str); i < len && check; ++i) {
    if (s21_isOper(input_str[i])) {
      if (i - 1 >= 0 && !s21_isdigit(input_str[i - 1]) &&
          input_str[i - 1] != ')' && input_str[i] != '-' &&
          input_str[i] != '+') {
        check = 0;
      }
      if (i + 1 < len && !s21_isdigit(input_str[i + 1]) &&
          input_str[i + 1] != '(' && !s21_CheckStartOfFunc(input_str[i + 1]) &&
          input_str[i + 1] != 'o') {
        check = 0;
      }
    }
  }
  return check;
}

int s21_DotCheck(char *input_str) {
  int check = 1, dot_count = 0;
  if (input_str[0] == '.' || input_str[strlen(input_str) - 1] == '.') {
    check = 0;
  }
  for (int i = 0, len = strlen(input_str); i < len && check; ++i) {
    if (input_str[i] == '.') {
      dot_count++;
    } else if (!s21_isdigit(input_str[i])) {
      dot_count = 0;
    }
    if (dot_count > 1) {
      check = 0;
    }
  }
  return check;
}

int s21_CheckParenthesisCorrectness(char *input_str) {
  int left = 0, right = 0;
  int check = 1;
  for (size_t i = 0, len = strlen(input_str); i < len && check; ++i) {
    if (input_str[i] == ')') {
      if (i + 1 < len && !s21_isOper(input_str[i + 1]) &&
          input_str[i + 1] != ')') {
        check = 0;
      }
      if (i >= 1 && s21_isOper(input_str[i - 1]) && input_str[i - 1] != ')') {
        check = 0;
      }
      right++;
    }

    if (input_str[i] == '(' && check) {
      if (i + 1 < len && input_str[i + 1] == ')') {
        check = 0;
      }
      if (i + 1 < len && input_str[i + 1] != '-' && input_str[i + 1] != '+' &&
          !s21_CheckStartOfFunc(input_str[i + 1]) &&
          !s21_isdigit(input_str[i + 1]) && input_str[i + 1] != '(') {
        check = 0;
      }
      left++;
    }
  }
  return check && (right == left);
}

void s21_ConvertUnary(char *input_str) {
  if (input_str[0] == '-') {
    input_str[0] = '~';
  }
  if (input_str[0] == '+') {
    input_str[0] = '`';
  }
  for (int i = 1, len = strlen(input_str); i < len; ++i) {
    if (input_str[i] == '-' && input_str[i - 1] == '(') {
      input_str[i] = '~';
    }

    if (input_str[i] == '+' && input_str[i - 1] == '(') {
      input_str[i] = '`';
    }
  }
}

int s21_CheckOperType(char ch) {
  return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' ||
         ch == s21_MOD;
}

// int main(void) {
//   // char input_str[] = "(2/(3+2)*5-7.5^3)mod3*sqrt(5)"; //-6.4287
//   //  char input_str[] = "sin(cos(1))^2*sin(3)";
//   //  char input_str[] = "-5*(-(-5-sin(-5^(-1))))"; //-24.00665
//   //  char input_str[] = "-1^2";
//   //    char input_str[] = "2.5+3-5.5";
//   //    char input_str[] = "5mod2";
//   //  char input_str[] = "sin(sin(1))*4";
//   //  char input_str[] = "4*sin(sin(~))";
//   //  char input_str[] = "(4*)";
//   char input_str[] = "-5+3";
//   char output_str[strlen(input_str) * 2];
//   printf("%s\n", s21_Solve(input_str, output_str));
//   // Stack *prefix = s21_ConvertToPostfix("2+3*5");
//   // s21_PrintStack(prefix);
//   // printf("%f\n", s21_Calculate(&prefix));
//   // s21_PrintStack(prefix);
// }
