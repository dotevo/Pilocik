#include "routewindow.h"
#include "ui_routewindow.h"

RouteWindow::RouteWindow(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::RouteWindow)
{
    ui->setupUi(this);
}

RouteWindow::~RouteWindow()
{
    delete ui;
}
