#include "useroptionswindow.h"
#include "ui_useroptionswindow.h"

UserOptionsWindow::UserOptionsWindow(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::UserOptionsWindow)
{
    ui->setupUi(this);
}

UserOptionsWindow::~UserOptionsWindow()
{
    delete ui;
}
