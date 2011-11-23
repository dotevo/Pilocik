#include "creditswindow.h"
#include "ui_creditswindow.h"
#include "navigationwindow.h"

CreditsWindow::CreditsWindow(NavigationWindow *parent) :
    QFullScreenFrame(parent),
    ui(new Ui::CreditsWindow)
{
    ui->setupUi(this);
}

CreditsWindow::~CreditsWindow()
{
    delete ui;
}

void CreditsWindow::on_backButton_clicked()
{
    setVisible(false);
    emit closed();
}
