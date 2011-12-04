#include "routewindow.h"
#include "ui_routewindow.h"
#include "navigationwindow.h"
#include "twidgetmanager.h"
#include "widgets/troutingprogresswidget.h"
#include "maprenderwidget.h"

#include <QDebug>

RouteWindow::RouteWindow(NavigationWindow *parent) :
    QFullScreenFrame(parent),
    ui(new Ui::RouteWindow)
{
    ui->setupUi(this);
    psw=0;
    routingManager = new RoutingManager();

    ui->label->setText(tr("ROUTE PLANNING"));

    //connect(routingManager, SIGNAL(NewRoute()), this, SLOT(NewRouteSlot()));

    startSet(0, 0, tr("Current position"));
    //connect(NavigationWindow::main->gps, SIGNAL(positionUpdate(GPSdata)), this, SLOT(FromCurrent(GPSdata)));
    targetSet(0, 0, tr("Current position"));
    //connect(NavigationWindow::main->gps, SIGNAL(positionUpdate(GPSdata)), this, SLOT(ToCurrent(GPSdata)));
    initThroughList();
}

RouteWindow::~RouteWindow()
{
    delete ui;
}

void RouteWindow::NewRouteSlot()
{
    NavigationWindow::main->mapRenderer->repaint();
}

void RouteWindow::FromCurrent(GPSdata gpsData)
{
    from.second.setLon(gpsData.lon);
    from.second.setLat(gpsData.lat);
}

void RouteWindow::ToCurrent(GPSdata gpsData)
{
    to.second.setLon(gpsData.lon);
    to.second.setLat(gpsData.lat);
}

void RouteWindow::initThroughList()
{
    ui->throughList->setColumnCount(3);
    ui->throughList->hideColumn(0);
    ui->throughList->hideColumn(1);
    ui->throughList->header()->hide();
    QListIterator< QPair< QString, PiLibocik::Position > > throughIterator(through);
    while(throughIterator.hasNext()) {
        QPair< QString, PiLibocik::Position > pair = throughIterator.next();

        QTreeWidgetItem* item = new QTreeWidgetItem(ui->throughList);
        item->setText(0, QString::number(pair.second.getLon()));
        item->setText(1, QString::number(pair.second.getLat()));
        item->setText(2, pair.first);
    }
}

PiLibocik::Position RouteWindow::getFrom()
{
    return from.second;
}

PiLibocik::Position RouteWindow::getTo()
{
    return to.second;
}

QList< PiLibocik::Position > RouteWindow::getThrough()
{
    QList< PiLibocik::Position > throughPositions;
    QListIterator< QPair< QString, PiLibocik::Position > > throughIterator(through);
    while(throughIterator.hasNext()) {
        QPair< QString, PiLibocik::Position > pair = throughIterator.next();
        throughPositions.push_back(pair.second);
    }
    return throughPositions;
}

void RouteWindow::on_fromButton_clicked()
{
    if(psw)
        delete psw;
    psw=new PointSelectionWindow(NavigationWindow::main,
                                 NavigationWindow::main->mapRenderer->getCoordinates().x(),
                                 NavigationWindow::main->mapRenderer->getCoordinates().y());
    psw->setVisible(true);
    connect(psw, SIGNAL(ok_clicked()), this, SLOT(pswClosed()));
    connect(psw, SIGNAL(positionChoosen(double,double,QString)), this, SLOT(startSet(double,double,QString)));
    setVisible(false);
}

void RouteWindow::on_toButton_clicked(){
    if(psw)
        delete psw;
    psw=new PointSelectionWindow(NavigationWindow::main,
                                 NavigationWindow::main->mapRenderer->getCoordinates().x(),
                                 NavigationWindow::main->mapRenderer->getCoordinates().y());
    psw->setVisible(true);
    connect(psw, SIGNAL(ok_clicked()), this, SLOT(pswClosed()));
    connect(psw, SIGNAL(back_clicked()), this, SLOT(pswClosed()));
    connect(psw, SIGNAL(positionChoosen(double,double,QString)), this, SLOT(targetSet(double,double,QString)));
    setVisible(false);
}

void RouteWindow::on_addButton_clicked()
{
    if(psw)
        delete psw;
    psw=new PointSelectionWindow(NavigationWindow::main,
                                 NavigationWindow::main->mapRenderer->getCoordinates().x(),
                                 NavigationWindow::main->mapRenderer->getCoordinates().y());
    psw->setVisible(true);
    connect(psw, SIGNAL(ok_clicked()), this, SLOT(pswClosed()));
    connect(psw, SIGNAL(positionChoosen(double,double,QString)), this, SLOT(addStop(double,double,QString)));
    setVisible(false);
}

void RouteWindow::on_routeBackButton_clicked(){
    setVisible(false);
    emit closed();
    ((TRoutingProgressWidget*) TWidgetManager::getInstance()->getWidget("RoutingProgress"))->stopCalculating();
}

void RouteWindow::on_okButton_clicked()
{
    setVisible(false);
    ((TRoutingProgressWidget *) TWidgetManager::getInstance()->getWidget("RoutingProgress"))->startCalculating();
    emit closed();
    routingManager->start();
}

void RouteWindow::startSet(double lon, double lat, QString name)
{
    disconnect(this, SLOT(FromCurrent(GPSdata)));
    ui->fromButton->setText(name);
    from.first = name;
    from.second.setLon(lon);
    from.second.setLat(lat);
    disconnect(this, SLOT(startSet(double,double,QString)));
    //Settings::getInstance()->addHistoryPoint(name, lon, lat);
}

void RouteWindow::targetSet(double lon, double lat, QString name)
{
    disconnect(this, SLOT(ToCurrent(GPSdata)));
    ui->toButton->setText(name);
    to.first = name;
    to.second.setLon(lon);
    to.second.setLat(lat);
    disconnect(this, SLOT(targetSet(double,double,QString)));
    //Settings::getInstance()->addHistoryPoint(name, lon, lat);
}

void RouteWindow::addStop(double lon, double lat, QString name)
{
    PiLibocik::Position pos(lon,lat);
    QPair< QString, PiLibocik::Position > pair(name, pos);
    through.push_back(pair);

    QTreeWidgetItem* item = new QTreeWidgetItem(ui->throughList);
    item->setText(0, QString::number(pair.second.getLon()));
    item->setText(1, QString::number(pair.second.getLat()));
    item->setText(2, pair.first);
    disconnect(this, SLOT(addStop(double,double,QString)));
    //Settings::getInstance()->addHistoryPoint(name, lon, lat);
}

void RouteWindow::pswClosed(){
    delete psw;
    psw=0;
    setVisible(true);
    NavigationWindow::main->mapRenderer->updateHint(MapRenderWidget::NormalHint);
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

void RouteWindow::on_clearThroughButton_clicked()
{
    through.clear();
    ui->throughList->clear();
}

void RouteWindow::on_fromClrButton_clicked()
{
    startSet(0, 0, tr("Current position"));
    connect(NavigationWindow::main->gps, SIGNAL(positionUpdate(GPSdata)), this, SLOT(FromCurrent(GPSdata)));
}

void RouteWindow::on_toClrButton_clicked()
{
    targetSet(0, 0, tr("Current position"));
    connect(NavigationWindow::main->gps, SIGNAL(positionUpdate(GPSdata)), this, SLOT(ToCurrent(GPSdata)));
}
