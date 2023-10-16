#ifndef DEPOSITWINDOW_H
#define DEPOSITWINDOW_H

#include <QMainWindow>

namespace Ui {
class DepositWindow;
}

class DepositWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit DepositWindow(QWidget *parent = nullptr);
  ~DepositWindow();

 private:
  Ui::DepositWindow *ui;
  QVector<double> rep_list;
  QVector<double> withd_list;
  int rep_row;
  int withd_row;

 private slots:
  void CalculateClicked();
  void AddRep();
  void AddWithd();
  void All_Clear();
};

#endif  // DEPOSITWINDOW_H
