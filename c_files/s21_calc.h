#ifndef S21_C7_SMARTCALC_SRC_C_FILES_S21_CALC
#define S21_C7_SMARTCALC_SRC_C_FILES_S21_CALC

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define s21_NUM 0
#define s21_OPER 1

#define s21_ANNUITY 0
#define s21_DIFFER 1

typedef enum s21_funcs {
  s21_COS = 'a',
  s21_SIN,
  s21_TAN,
  s21_ACOS,
  s21_ASIN,
  s21_ATAN,
  s21_SQRT,
  s21_LN,
  s21_LOG,
  s21_MOD
} s21_funcs;

typedef struct s21_Stack {
  double val;
  char oper;
  struct s21_Stack *next;
  int type;
} s21_Stack;

typedef struct s21_Credit {
  double total_credit;
  int term;
  double rate;
  int type;
  double monthly;
  double overpay;
  double total_payment;
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

void s21_pop(s21_Stack **top);
void s21_push(s21_Stack **top, double val, char oper, int type);
void s21_destroy(s21_Stack *top);

char *s21_Solve(char *input_str, char *output_str);
double s21_Calculate(s21_Stack **postfixx);
s21_Stack *s21_ConvertToPostfix(char *input_str);
void s21_ReverseList(s21_Stack **prefix);
int s21_isdigit(char ch);
void s21_GetNum(char *str, int *i, s21_Stack **output);
int s21_CheckFunc(const char *input_str, char *func_name, int *i);
int s21_isOper(char ch);
void s21_HandleOper(s21_Stack **stack, s21_Stack **output, char oper);
int s21_CheckPrecedence(char oper);
void s21_HandleRightParenthesis(s21_Stack **stack, s21_Stack **output);
int s21_isFunc(char ch);
int s21_NormalizeStr(char *input_str);
int s21_CheckParenthesisCorrectness(char *input_str);
void s21_ConvertUnary(char *input_str);
int s21_DotCheck(char *input_str);
int s21_OperCheck(char *input_str);
int s21_NumberCheck(char *input_str);
int s21_CheckOperType(char ch);
void s21_HandleOperCalc(double *value, double stack_val, char oper);
int s21_CheckStartOfFunc(char ch);
int s21_isOperWithoutUnary(char ch);

void s21_CreditCalc(s21_Credit *data, double *monthly_interest_p);
void s21_DepositCalc(s21_Deposit *data, double rep_summ, double withd_summ);

#endif