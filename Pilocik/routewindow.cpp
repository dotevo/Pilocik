#include "routewindow.h"
#include "ui_routewindow.h"
#include "navigationwindow.h"

RouteWindow::RouteWindow(NavigationWindow *parent) :
    QFullScreenFrame(parent),
    ui(new Ui::RouteWindow)
{
    ui->setupUi(this);
    psw=0;

    ui->label->setText(tr("ROUTE PLANNING"));
}

RouteWindow::~RouteWindow(){
    delete ui;
}

void RouteWindow::on_toButton_clicked(){
    if(psw)
        delete psw;
    psw=new PointSelectionWindow(NavigationWindow::main,
                                 NavigationWindow::main->mapRenderer->getCoordinates().x(),
                                 NavigationWindow::main->mapRenderer->getCoordinates().y());
    psw->setVisible(true);
    connect(psw, SIGNAL(ok_clicked()), this, SLOT(pswClosed()));
    connect(psw, SIGNAL(positionChoosen(double,double,QString)), this, SLOT(targetSet(double,double,QString)));
    setVisible(false);
}

void RouteWindow::on_routeBackButton_clicked(){
    setVisible(false);
    emit windowClosed();
}

void RouteWindow::targetSet(double lon, double lat, QString name)
{
    ui->toButton->setText(name);
    Settings::getInstance()->addHistoryPoint(name, lon, lat);
}

void RouteWindow::pswClosed(){
    delete psw;
    psw=0;
    setVisible(true);
}

void RouteWindow::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
