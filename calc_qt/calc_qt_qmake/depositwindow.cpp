#include "depositwindow.h"

#include <numeric>

#include "mainwindow.h"
#include "ui_depositwindow.h"

DepositWindow::DepositWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::DepositWindow) {
  ui->setupUi(this);
  connect(ui->button_eval, SIGNAL(clicked()), this, SLOT(CalculateClicked()));
  connect(ui->pushButton_replenish, SIGNAL(clicked()), this, SLOT(AddRep()));
  connect(ui->pushButton_withd, SIGNAL(clicked()), this, SLOT(AddWithd()));
  connect(ui->button_clear, SIGNAL(clicked()), this, SLOT(All_Clear()));
  this->rep_row = 0;
  this->withd_row = 0;
  this->rep_list.clear();
  this->withd_list.clear();
}

DepositWindow::~DepositWindow() { delete ui; }

void DepositWindow::CalculateClicked() {
  bool check = true;
  if (ui->lineEdit_deposit_amount->text().isEmpty() ||
      ui->lineEdit_interest_rate->text().isEmpty() ||
      ui->lineEdit_periodicity->text().isEmpty() ||
      ui->lineEdit_tax_rate->text().isEmpty() ||
      ui->lineEdit_term->text().isEmpty()) {
    check = false;
  }
  if (!s21_isDouble(ui->lineEdit_deposit_amount->text()) ||
      !s21_isDouble(ui->lineEdit_interest_rate->text()) ||
      !s21_isDouble(ui->lineEdit_periodicity->text()) ||
      !s21_isDouble(ui->lineEdit_tax_rate->text()) ||
      !s21_isDouble(ui->lineEdit_term->text())) {
    check = false;
  }

  if (check) {
    s21_Deposit vals = {0};
    vals.amount = ui->lineEdit_deposit_amount->text().toDouble();
    vals.term = ui->lineEdit_term->text().toInt();
    vals.interest_rate = ui->lineEdit_interest_rate->text().toDouble();
    vals.tax_rate = ui->lineEdit_tax_rate->text().toDouble();
    vals.periodicity = ui->lineEdit_periodicity->text().toInt();

    if (vals.amount < 0 || vals.term < 0 || vals.interest_rate < 0 ||
        vals.tax_rate < 0 || vals.periodicity < 0) {
      check = false;
    }

    if (check) {
      vals.capitalization = ui->radio_cap_yes->isChecked() ? 1 : 0;
      double rep_summ = 0, withd_summ = 0;
      if (!this->rep_list.empty()) {
        rep_summ = std::reduce(this->rep_list.begin(), this->rep_list.end());
      }
      if (!this->withd_list.empty()) {
        withd_summ =
            std::reduce(this->withd_list.begin(), this->withd_list.end());
      }
      s21_DepositCalc(&vals, rep_summ, withd_summ);

      char buff[1028];
      std::sprintf(buff, "%.2f", vals.accrued_interest);
      ui->label_accrued->setText(QString::fromStdString(buff));
      std::sprintf(buff, "%.2f", vals.tax_amount);
      ui->label_tax_sum->setText(QString::fromStdString(buff));
      std::sprintf(buff, "%.2f", vals.deposit_amount);
      ui->label_deposit_sum->setText(QString::fromStdString(buff));
    }
  }

  if (!check) {
    ui->error_label->setText("Incorrect input");
  }
}

void DepositWindow::AddRep() {
  bool check = true;
  if (ui->lineEdit_replenish->text().isEmpty() ||
      !s21_isDouble(ui->lineEdit_replenish->text())) {
    check = false;
  }

  if (check) {
    double rep_val = ui->lineEdit_replenish->text().toDouble();
    this->rep_list.push_back(rep_val);

    ui->tableWidget_replenish->setColumnCount(1);
    char buff[1028];
    std::sprintf(buff, "%.2f", rep_val);
    QString str_val = QString::fromStdString(buff);
    ui->tableWidget_replenish->insertRow(this->rep_row);

    QTableWidgetItem *double_item = new QTableWidgetItem(str_val);
    ui->tableWidget_replenish->setItem(this->rep_row, 0, double_item);
    this->rep_row++;

    ui->tableWidget_replenish->setHorizontalHeaderLabels(QStringList()
                                                         << "Пополнения");
    ui->tableWidget_replenish->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);
  }

  if (!check) {
    ui->error_label->setText("Incorrect input");
  }
}

void DepositWindow::AddWithd() {
  bool check = true;
  if (ui->lineEdit_withd->text().isEmpty() ||
      !s21_isDouble(ui->lineEdit_withd->text())) {
    check = false;
  }

  if (check) {
    double withd_val = ui->lineEdit_withd->text().toDouble();
    this->withd_list.push_back(withd_val);

    ui->tableWidget_withdr->setColumnCount(1);
    char buff[1028];
    std::sprintf(buff, "%.2f", withd_val);
    QString str_val = QString::fromStdString(buff);
    ui->tableWidget_withdr->insertRow(this->withd_row);

    QTableWidgetItem *double_item = new QTableWidgetItem(str_val);
    ui->tableWidget_withdr->setItem(this->withd_row, 0, double_item);
    this->withd_row++;

    ui->tableWidget_withdr->setHorizontalHeaderLabels(QStringList()
                                                      << "Пополнения");
    ui->tableWidget_withdr->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);
  }

  if (!check) {
    ui->error_label->setText("Incorrect input");
  }
}

void DepositWindow::All_Clear() {
  ui->lineEdit_deposit_amount->clear();
  ui->lineEdit_interest_rate->clear();
  ui->lineEdit_periodicity->clear();
  ui->lineEdit_replenish->clear();
  ui->lineEdit_tax_rate->clear();
  ui->lineEdit_term->clear();
  ui->lineEdit_withd->clear();
  ui->label_accrued->setText("");
  ui->label_tax_sum->setText("");
  ui->label_deposit_sum->setText("");
  ui->error_label->setText("");
  ui->tableWidget_replenish->clearContents();
  ui->tableWidget_replenish->setRowCount(0);
  ui->tableWidget_withdr->clearContents();
  ui->tableWidget_withdr->setRowCount(0);
  this->rep_row = 0;
  this->withd_row = 0;
  this->rep_list.clear();
  this->withd_list.clear();
}
