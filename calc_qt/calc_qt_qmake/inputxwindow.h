#ifndef INPUTXWINDOW_H
#define INPUTXWINDOW_H

#include <QMainWindow>

namespace Ui {
class InputXWindow;
}

class InputXWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit InputXWindow(QWidget *parent = nullptr);
  ~InputXWindow();

 private:
  Ui::InputXWindow *ui;
};

#endif  // INPUTXWINDOW_H
