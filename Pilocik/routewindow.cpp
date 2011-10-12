#include "routewindow.h"
#include "ui_routewindow.h"
#include "navigationwindow.h"

RouteWindow::RouteWindow(NavigationWindow *parent) :
    QFullScreenFrame(parent),
    ui(new Ui::RouteWindow)
{
    ui->setupUi(this);
    psw=0;
}

RouteWindow::~RouteWindow(){
    delete ui;
}

void RouteWindow::on_toButton_clicked(){
    if(psw)
        delete psw;
    psw=new PointSelectionWindow(NavigationWindow::main);
    psw->setVisible(true);
    connect(psw, SIGNAL(ok_clicked()), this, SLOT(pswClosed()));
}

void RouteWindow::on_routeBackButton_clicked(){
    setVisible(false);
}

void RouteWindow::pswClosed(){
    delete psw;
    psw=0;
}
