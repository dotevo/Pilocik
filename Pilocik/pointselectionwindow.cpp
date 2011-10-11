#include "pointselectionwindow.h"
#include "ui_pointselectionwindow.h"
#include "navigationwindow.h"

PointSelectionWindow::PointSelectionWindow(NavigationWindow *parent) :
    ui(new Ui::PointSelectionWindow),
    QFullScreenFrame(parent)
{
    ui->setupUi(this);
    sizeChanged(NavigationWindow::main);
}

PointSelectionWindow::~PointSelectionWindow(){
    delete ui;
}

void PointSelectionWindow::on_okButton_clicked(){
    emit ok_clicked();
}
