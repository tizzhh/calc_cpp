#include "graphwindow.h"

#include "mainwindow.h"
#include "ui_graphwindow.h"

GraphWindow::GraphWindow(const std::string &input, double x_min, double x_max,
                         double y_min, double y_max, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::GraphWindow) {
  ui->setupUi(this);
  QVector<double> xData, yData;
  double left_range = std::min(x_min, y_min),
         right_range = std::max(x_max, y_max);
  double step = CalculateStep(left_range, right_range);
  std::string output = input;
  char *to_unary = const_cast<char *>(output.c_str());
  s21_ConvertUnary(to_unary);
  s21_Stack *prefix = s21_ConvertToPostfix(to_unary);
  for (int i = 0; i <= (x_max - x_min) / step; ++i) {
    double y = 0.0;
    double x = x_min + i * step;
    s21_push(&prefix, x, 'x', s21_NUM);
    y = s21_Calculate(&prefix);
    xData.push_back(x);
    yData.push_back(y);
  }
  s21_destroy(prefix);

  QPen pen;
  pen.setColor(QColor(136, 8, 8));
  pen.setWidth(1);
  ui->custom_plot->clearGraphs();
  ui->custom_plot->addGraph();
  ui->custom_plot->graph(0)->setData(xData, yData);
  ui->custom_plot->xAxis->setLabel("X");
  ui->custom_plot->yAxis->setLabel("Y");
  ui->custom_plot->xAxis->setRange(x_min, x_max);
  ui->custom_plot->yAxis->setRange(y_min, y_max);
  ui->custom_plot->graph(0)->setPen(pen);
  ui->custom_plot->replot();
}

double CalculateStep(double num1, double num2) {
  int num = int(abs(num1) + abs(num2));
  int count = 0;
  while (num != 0) {
    ++count;
    num /= 10;
  }
  return 1 / std::pow(10, count);
}

GraphWindow::~GraphWindow() { delete ui; }
