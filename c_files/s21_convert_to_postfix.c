#include "s21_calc.h"

s21_Stack *s21_ConvertToPostfix(char *input_str) {
  s21_Stack *stack = NULL;
  s21_Stack *output = NULL;
  char func_name = 0;
  int len = (int)strlen(input_str);
  for (int i = 0; i < len; ++i) {
    if (s21_isdigit(input_str[i])) {
      s21_GetNum(input_str, &i, &output);
    } else if (input_str[i] == '(') {
      s21_push(&stack, 0, '(', s21_OPER);
    } else if (s21_CheckFunc(input_str, &func_name, &i)) {
      s21_push(&stack, 0, func_name, s21_OPER);
      func_name = 0;
    } else if (s21_isOper(input_str[i])) {
      s21_HandleOper(&stack, &output, input_str[i]);
    } else if (input_str[i] == ')') {
      s21_HandleRightParenthesis(&stack, &output);
    } else if (input_str[i] == 'x') {
      s21_push(&output, 0, 'x', s21_NUM);
    }
  }
  while (stack != NULL) {
    s21_push(&output, 0, stack->oper, s21_OPER);
    s21_pop(&stack);
  }
  s21_destroy(stack);
  s21_ReverseList(&output);
  return output;
}

void s21_GetNum(char *str, int *i, s21_Stack **output) {
  char *num = calloc(strlen(str), sizeof(char));
  int pos = 0;
  while (s21_isdigit(str[*i]) || str[*i] == '.') {
    num[pos++] = str[(*i)++];
  }
  s21_push(output, atof(num), 0, s21_NUM);
  free(num);
  (*i)--;
}

int s21_CheckFunc(const char *input_str, char *func_name, int *i) {
  int check = 1;
  if (input_str[*i] == 'c') {
    *func_name = s21_COS;
    *i += 2;
  } else if (input_str[*i] == 's' && input_str[*i + 1] == 'i') {
    *func_name = s21_SIN;
    *i += 2;
  } else if (input_str[*i] == 't') {
    *func_name = s21_TAN;
    *i += 2;
  } else if (input_str[*i] == 'a' && input_str[*i + 1] == 'c') {
    *func_name = s21_ACOS;
    *i += 3;
  } else if (input_str[*i] == 'a' && input_str[*i + 1] == 's') {
    *func_name = s21_ASIN;
    *i += 3;
  } else if (input_str[*i] == 'a' && input_str[*i + 1] == 't') {
    *func_name = s21_ATAN;
    *i += 3;
  } else if (input_str[*i] == 's' && input_str[*i + 1] == 'q') {
    *func_name = s21_SQRT;
    *i += 3;
  } else if (input_str[*i] == 'l' && input_str[*i + 1] == 'n') {
    *func_name = s21_LN;
    *i += 1;
  } else if (input_str[*i] == 'l' && input_str[*i + 1] == 'o') {
    *func_name = s21_LOG;
    *i += 2;
  } else if (input_str[*i] == 'm') {
    *func_name = s21_MOD;
    *i += 2;
  } else {
    check = 0;
  }
  return check;
}

void s21_HandleRightParenthesis(s21_Stack **stack, s21_Stack **output) {
  while (stack != NULL && (*stack)->oper != '(') {
    s21_push(output, 0, (*stack)->oper, s21_OPER);
    s21_pop(stack);
  }
  if (stack != NULL && *stack != NULL) s21_pop(stack);
  if (stack != NULL && *stack != NULL && s21_isFunc((*stack)->oper)) {
    s21_push(output, 0, (*stack)->oper, s21_OPER);
    s21_pop(stack);
  }
}

void s21_HandleOper(s21_Stack **stack, s21_Stack **output, char oper) {
  if (*stack == NULL || oper == '^') {
    s21_push(stack, 0, oper, s21_OPER);
  } else {
    while (*stack != NULL &&
           s21_CheckPrecedence(oper) <= s21_CheckPrecedence((*stack)->oper)) {
      s21_push(output, 0, (*stack)->oper, s21_OPER);
      s21_pop(stack);
    }
    s21_push(stack, 0, oper, s21_OPER);
  }
}

int s21_CheckPrecedence(char oper) {
  int precedence = 0;
  if (oper == s21_COS || oper == s21_SIN || oper == s21_TAN ||
      oper == s21_ACOS || oper == s21_ASIN || oper == s21_ATAN ||
      oper == s21_SQRT || oper == s21_LN || oper == s21_LOG ||
      oper == s21_MOD) {
    precedence = 5;
  } else if (oper == '~' || oper == '`') {
    precedence = 4;
  } else if (oper == '*' || oper == '/') {
    precedence = 2;
  } else if (oper == '-' || oper == '+') {
    precedence = 1;
  }
  return precedence;
}

int s21_isdigit(char ch) { return ch >= '0' && ch <= '9'; }

int s21_isOper(char ch) {
  return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' ||
         ch == 'm' || ch == '~' || ch == '`' || ch == s21_MOD;
}

int s21_isOperWithoutUnary(char ch) {
  return ch == '*' || ch == '/' || ch == '^' || ch == 'm' || ch == s21_MOD;
}

int s21_isFunc(char ch) {
  return ch == s21_COS || ch == s21_SIN || ch == s21_TAN || ch == s21_ACOS ||
         ch == s21_ASIN || ch == s21_ATAN || ch == s21_SQRT || ch == s21_LN ||
         ch == s21_LOG;
}

int s21_CheckStartOfFunc(char ch) {
  return ch == 'c' || ch == 's' || ch == 't' || ch == 'a' || ch == 'q' ||
         ch == 'l';
}