#include "s21_calc.h"

void s21_pop(s21_Stack **top) {
  s21_Stack *temp = *top;
  *top = (*top)->next;
  free(temp);
}

void s21_push(s21_Stack **top, double val, char oper, int type) {
  s21_Stack *new_top = malloc(sizeof(s21_Stack));
  new_top->val = val;
  new_top->oper = oper;
  new_top->next = *top;
  new_top->type = type;
  *top = new_top;
}

void s21_destroy(s21_Stack *top) {
  s21_Stack *ptr = top;
  while (ptr != NULL) {
    s21_Stack *next = ptr->next;
    free(ptr);
    ptr = next;
  }
}

void s21_PrintStack(s21_Stack *res) {
  s21_Stack *ptr = res;
  while (ptr != NULL) {
    if (ptr->type == s21_OPER) {
      printf("%c ", ptr->oper);
    } else if (ptr->type == s21_NUM) {
      printf("%f ", ptr->val);
    }
    ptr = ptr->next;
  }
  printf("\n");
}
