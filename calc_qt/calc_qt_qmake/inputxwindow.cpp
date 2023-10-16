#include "inputxwindow.h"

#include "ui_inputxwindow.h"

InputXWindow::InputXWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::InputXWindow) {
  ui->setupUi(this);
}

InputXWindow::~InputXWindow() { delete ui; }
