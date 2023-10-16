#include "s21_calc.h"

void s21_CreditCalc(s21_Credit *data, double *monthly_interest_p) {
  data->rate /= 100.0;
  double monthly_rate = data->rate / 12.0;
  if (data->type == s21_ANNUITY) {
    data->monthly = data->total_credit *
                    ((monthly_rate * pow(1 + monthly_rate, data->term)) /
                     (pow(1 + monthly_rate, data->term) - 1));
    data->total_payment = data->monthly * data->term;
    data->overpay = (data->monthly * data->term) - data->total_credit;
  } else if (data->type == s21_DIFFER) {
    double monthly_total_credit = data->total_credit / data->term;
    for (int i = 0; i < data->term; ++i) {
      data->monthly = data->total_credit / data->term;
      monthly_interest_p[i] =
          (data->total_credit - (monthly_total_credit * (i + 1 - 1))) *
          monthly_rate;
      monthly_interest_p[i] += monthly_total_credit;
      data->total_payment += monthly_interest_p[i];
    }
    data->overpay = data->total_payment - data->total_credit;
  }
}