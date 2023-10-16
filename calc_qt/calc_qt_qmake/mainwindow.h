#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPen>
#include <QVector>
#include <cmath>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

extern "C" {
#include "../../c_files/s21_calc.h"
}

bool s21_isDouble(QString str);
bool s21_checkXPos(const std::string &str);
bool s21_isNumber(const std::string &str);

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;
  bool prev_solved;

 private slots:
  void inputButtonClicked();
  void clearInput();
  void solveClicked();
  void graphClicked();
  void depositClicked();
  void creditClicked();
};
#endif  // MAINWINDOW_H
