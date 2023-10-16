#include "creditwindow.h"

#include "mainwindow.h"
#include "ui_creditwindow.h"

creditwindow::creditwindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::creditwindow) {
  ui->setupUi(this);
  connect(ui->button_eval, SIGNAL(clicked()), this, SLOT(CalculateClicked()));
}

creditwindow::~creditwindow() { delete ui; }

void creditwindow::CalculateClicked() {
  All_Clear();
  ui->tableWidget->clearContents();
  ui->tableWidget->setRowCount(0);
  bool check = true;
  if (ui->lineEdit_total_credit->text().isEmpty() ||
      ui->lineEdit_term->text().isEmpty() ||
      ui->lineEdit_rate->text().isEmpty()) {
    check = false;
  }
  if (!s21_isNumber(ui->lineEdit_term->text().toStdString()) ||
      !s21_isDouble(ui->lineEdit_rate->text()) ||
      !s21_isDouble(ui->lineEdit_total_credit->text())) {
    check = false;
  }

  s21_Credit vals = {0};
  if (check) {
    vals.total_credit = ui->lineEdit_total_credit->text().toDouble();
    vals.term = ui->lineEdit_term->text().toInt();
    vals.rate = ui->lineEdit_rate->text().toDouble();
    vals.type = ui->radio_annuity->isChecked() ? s21_ANNUITY : s21_DIFFER;

    if (vals.total_credit < 0 || vals.term < 0 || vals.rate < 0) {
      check = false;
    }

    if (check) {
      double monthly_interest_p[ui->lineEdit_term->text().toInt()];
      s21_CreditCalc(&vals, monthly_interest_p);
      char buff[1028];
      if (vals.type == s21_DIFFER) {
        ui->tableWidget->setColumnCount(1);
        int row_count = 0;
        for (double val : monthly_interest_p) {
          std::sprintf(buff, "%.2f", val);
          QString str_val = QString::fromStdString(buff);
          ui->tableWidget->insertRow(row_count);

          QTableWidgetItem *double_item = new QTableWidgetItem(str_val);
          ui->tableWidget->setItem(row_count, 0, double_item);
          ++row_count;
        }
        ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Платеж");
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(
            QHeaderView::Stretch);
      }
      std::sprintf(buff, "%.2f", vals.monthly);
      ui->montly_pay->setText(QString::fromStdString(buff));
      std::sprintf(buff, "%.2f", vals.overpay);
      ui->overpay->setText(QString::fromStdString(buff));
      std::sprintf(buff, "%.2f", vals.total_payment);
      ui->total_payment->setText(QString::fromStdString(buff));
    }
  }

  if (!check) {
    ui->error_label->setText("Incorrect input");
  }
}

void creditwindow::All_Clear() {
  ui->montly_pay->setText("");
  ui->overpay->setText("");
  ui->total_payment->setText("");
  ui->error_label->setText("");
}

bool s21_isNumber(const std::string &str) {
  bool check = true;
  for (char const &ch : str) {
    if (!std::isdigit(ch)) {
      check = false;
      break;
    }
  }
  return check;
}
