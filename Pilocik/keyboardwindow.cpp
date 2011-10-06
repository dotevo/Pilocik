#include "keyboardwindow.h"
#include "ui_keyboardwindow.h"

KeyboardWindow::KeyboardWindow(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::KeyboardWindow)
{
    ui->setupUi(this);
}

KeyboardWindow::~KeyboardWindow()
{
    delete ui;
}
