#include "s21_calc.h"

void s21_DepositCalc(s21_Deposit *data, double rep_summ, double withd_summ) {
  data->tax_rate /= 100;
  data->interest_rate /= 100;
  for (int i = 0; i < data->term; ++i) {
    double cum_amount = data->amount * data->interest_rate / data->periodicity; // надо починить периодичность (при периодичности 12 - норм, при другой - нет)
    data->accrued_interest += cum_amount;
    data->amount += cum_amount * data->capitalization;
  }

  data->tax_amount = data->tax_rate * data->accrued_interest;

  data->deposit_amount = data->amount + rep_summ - withd_summ +
                         (data->capitalization ? 0 : data->accrued_interest);
}