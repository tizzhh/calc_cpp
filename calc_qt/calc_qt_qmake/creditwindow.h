#ifndef CREDITWINDOW_H
#define CREDITWINDOW_H

#include <QMainWindow>

namespace Ui {
class creditwindow;
}

class creditwindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit creditwindow(QWidget *parent = nullptr);
  ~creditwindow();

 private:
  Ui::creditwindow *ui;

 private slots:
  void CalculateClicked();
  void All_Clear();
};

#endif  // CREDITWINDOW_H
