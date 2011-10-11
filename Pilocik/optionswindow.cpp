#include "optionswindow.h"
#include "ui_optionswindow.h"
#include "mapoptionswindow.h"
#include "navigationwindow.h"

OptionsWindow::OptionsWindow(NavigationWindow *parent) :
    QFullScreenFrame(parent),
    ui(new Ui::OptionsWindow)
{
    ui->setupUi(this);
    moWin=new MapOptionsWindow(parent);
    sizeChanged((QWidget*)parent);
    moWin->setVisible(false);
}

OptionsWindow::~OptionsWindow(){
    delete ui;
    delete moWin;
}

void OptionsWindow::on_backButton_clicked(){
    setVisible(false);
}

void OptionsWindow::on_mapButton_clicked(){
    moWin->setVisible(true);
    moWin->raise();

}
