#include "pointselectionwindow.h"
#include "ui_pointselectionwindow.h"

PointSelectionWindow::PointSelectionWindow(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PointSelectionWindow)
{
    ui->setupUi(this);
}

PointSelectionWindow::~PointSelectionWindow()
{
    delete ui;
}
