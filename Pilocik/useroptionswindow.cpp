#include "useroptionswindow.h"
#include "ui_useroptionswindow.h"
#include "navigationwindow.h"

UserOptionsWindow::UserOptionsWindow(NavigationWindow *parent) :
    QFullScreenFrame(parent),
    ui(new Ui::UserOptionsWindow)
{
    ui->setupUi(this);
    sizeChanged((QWidget*)parent);
}

UserOptionsWindow::~UserOptionsWindow(){
    delete ui;
}

void UserOptionsWindow::on_okButton_clicked(){
    setVisible(false);
    emit closed();
}
