#include "mainwindow.h"

#include <iostream>

#include "creditwindow.h"
#include "depositwindow.h"
#include "graphwindow.h"
#include "inputxwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  prev_solved = false;
  connect(ui->button_0, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));
  connect(ui->button_1, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));
  connect(ui->button_2, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));
  connect(ui->button_3, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));
  connect(ui->button_4, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));
  connect(ui->button_5, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));
  connect(ui->button_6, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));
  connect(ui->button_7, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));
  connect(ui->button_8, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));
  connect(ui->button_9, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));
  connect(ui->button_acos, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));
  connect(ui->button_asin, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));
  connect(ui->button_atan, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));
  connect(ui->button_cos, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));
  connect(ui->button_div, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));
  connect(ui->button_dot, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));
  connect(ui->button_left_par, SIGNAL(clicked()), this,
          SLOT(inputButtonClicked()));
  connect(ui->button_ln, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));
  connect(ui->button_log, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));
  connect(ui->button_minus, SIGNAL(clicked()), this,
          SLOT(inputButtonClicked()));
  connect(ui->button_mod, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));
  connect(ui->button_mult, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));
  connect(ui->button_plus, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));
  connect(ui->button_pow, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));
  connect(ui->button_right_par, SIGNAL(clicked()), this,
          SLOT(inputButtonClicked()));
  connect(ui->button_sin, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));
  connect(ui->button_sqrt, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));
  connect(ui->button_tan, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));
  connect(ui->button_x, SIGNAL(clicked()), this, SLOT(inputButtonClicked()));

  connect(ui->button_graph, SIGNAL(clicked()), this, SLOT(graphClicked()));
  connect(ui->button_clear, SIGNAL(clicked()), this, SLOT(clearInput()));
  connect(ui->button_solve, SIGNAL(clicked()), this, SLOT(solveClicked()));
  connect(ui->button_credit, SIGNAL(clicked()), this, SLOT(creditClicked()));
  connect(ui->button_deposit, SIGNAL(clicked()), this, SLOT(depositClicked()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::inputButtonClicked() {
  QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());

  if (this->prev_solved) {
    clearInput();
    this->prev_solved = false;
  }
  if (clickedButton) {
    QString buttonText = clickedButton->text();
    QString currentOutput = ui->OutputLabel->text() + buttonText;
    ui->OutputLabel->setText(currentOutput);
  }
}

void MainWindow::clearInput() { ui->OutputLabel->setText(""); }

void MainWindow::solveClicked() {
  std::string output = ui->OutputLabel->text().toStdString();
  if (ui->OutputLabel->text().contains('x')) {
    if (!s21_checkXPos(output)) {
      ui->OutputLabel->setText("ERROR, please provide correct input");
      this->prev_solved = true;
    } else if (s21_isDouble(ui->lineEdit->text())) {
      std::string x_val = ui->lineEdit->text().toStdString();
      size_t found = output.find('x');
      while (found != std::string::npos) {
        output.replace(found, 1, x_val);
        found = output.find('x', found + 1);
      }
      char *infix_str = const_cast<char *>(output.c_str());
      char result[1000] = {0};
      s21_Solve(infix_str, result);
      ui->OutputLabel->setText(QString::fromStdString(result));
      this->prev_solved = true;
    } else {
      InputXWindow *inputWindow = new InputXWindow(this);
      inputWindow->show();
    }
  } else {
    char *infix_str = const_cast<char *>(output.c_str());
    char result[1000] = {0};
    s21_Solve(infix_str, result);
    ui->OutputLabel->setText(QString::fromStdString(result));
    this->prev_solved = true;
  }
}

bool s21_checkXPos(const std::string &str) {
  bool check = true;
  size_t x_pos = str.find('x');
  while (x_pos != std::string::npos && check) {
    if (x_pos > 0 && (!s21_isOper(str[x_pos - 1]) && str[x_pos - 1] != '(')) {
      check = false;
    }
    if (x_pos < str.length() - 1 &&
        (!s21_isOper(str[x_pos + 1]) && str[x_pos + 1] != ')')) {
      check = false;
    }
    x_pos = str.find('x', x_pos + 1);
  }
  return check;
}

bool s21_isDouble(QString str) {
  std::string stdstr = str.toStdString();
  std::istringstream iss(stdstr);
  double temp;
  iss >> std::noskipws >> temp;
  return iss.eof() && !iss.fail();
}

void MainWindow::graphClicked() {
  std::string output = ui->OutputLabel->text().toStdString();
  std::string temp = output;
  size_t found = temp.find('x');
  while (found != std::string::npos) {
    temp.replace(found, 1, "(" + std::to_string(5) + ")");
    found = temp.find('x', found + 1);
  }
  bool check = s21_NormalizeStr(const_cast<char *>(temp.c_str()));
  bool check_range = true;
  if (ui->lineEdit_min_x->text().isEmpty() ||
      ui->lineEdit_max_x->text().isEmpty() ||
      ui->lineEdit_min_y->text().isEmpty() ||
      ui->lineEdit_max_y->text().isEmpty()) {
    check_range = false;
  }
  double x_min = s21_isDouble(ui->lineEdit_min_x->text())
                     ? ui->lineEdit_min_x->text().toDouble()
                     : -10000000;
  double x_max = s21_isDouble(ui->lineEdit_max_x->text())
                     ? ui->lineEdit_max_x->text().toDouble()
                     : 10000000;
  double y_min = s21_isDouble(ui->lineEdit_min_y->text())
                     ? ui->lineEdit_min_y->text().toDouble()
                     : -10000000;
  double y_max = s21_isDouble(ui->lineEdit_max_y->text())
                     ? ui->lineEdit_max_y->text().toDouble()
                     : 10000000;
  if (x_min < -1000000 || x_max > 1000000 || y_min < -1000000 ||
      y_max > 1000000) {
    check_range = false;
  }
  if ((x_min >= x_max) || (y_min >= y_max)) {
    check_range = false;
  }
  if (!check) {
    ui->OutputLabel->setText("ERROR, please provide correct input");
    this->prev_solved = true;
  } else if (!check_range) {
    ui->OutputLabel->setText("ERROR, incorrect range");
    this->prev_solved = true;
  } else {
    if (ui->OutputLabel->text().contains('x')) {
      if (s21_checkXPos(output)) {
        GraphWindow *graphWindow =
            new GraphWindow(output, x_min, x_max, y_min, y_max, this);
        graphWindow->show();
        this->prev_solved = true;
      } else {
        ui->OutputLabel->setText("ERROR, please provide correct input");
        this->prev_solved = true;
      }
    } else {
      GraphWindow *graphWindow =
          new GraphWindow(output, x_min, x_max, y_min, y_max, this);
      graphWindow->show();
      this->prev_solved = true;
    }
  }
}

void MainWindow::depositClicked() {
  DepositWindow *deposit = new DepositWindow();
  deposit->show();
}

void MainWindow::creditClicked() {
  creditwindow *credit = new creditwindow();
  credit->show();
}
