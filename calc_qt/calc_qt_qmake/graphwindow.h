#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

#include <QMainWindow>

extern "C" {
#include "../../c_files/s21_calc.h"
}

double CalculateStep(double range1, double range2);

namespace Ui {
class GraphWindow;
}

class GraphWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit GraphWindow(const std::string &input, double x_min, double x_max,
                       double y_min, double y_max, QWidget *parent = nullptr);
  ~GraphWindow();

 private:
  Ui::GraphWindow *ui;
};

#endif  // GRAPHWINDOW_H
