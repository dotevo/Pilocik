#include "routewindow.h"
#include "ui_routewindow.h"
#include "navigationwindow.h"
#include "twidgetmanager.h"
#include "widgets/troutingprogresswidget.h"

#include <QDebug>

RouteWindow::RouteWindow(NavigationWindow *parent) :
    QFullScreenFrame(parent),
    ui(new Ui::RouteWindow)
{
    ui->setupUi(this);
    psw=0;
    routingManager = new RoutingManager();
    qDebug() << "start";

    ui->label->setText(tr("ROUTE PLANNING"));

    // TODO: Nie wiem dlaczego nie dziala...
    connect(routingManager->getRouting(), SIGNAL(RoutingProgress(int)), (TRoutingProgressWidget *) TWidgetManager::getInstance()->getWidget("RoutingProgress"), SLOT(setProgress(int)));

    initThroughList();
}

RouteWindow::~RouteWindow()
{
    delete ui;
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
    emit closed();
    ((TRoutingProgressWidget*) TWidgetManager::getInstance()->getWidget("RoutingProgress"))->stopCalculating();
}

void RouteWindow::on_okButton_clicked()
{
    qDebug() << "start";
    setVisible(false);
    ((TRoutingProgressWidget *) TWidgetManager::getInstance()->getWidget("RoutingProgress"))->startCalculating();
    emit closed();
    routingManager->start();
}

void RouteWindow::startSet(double lon, double lat, QString name)
{
    ui->fromButton->setText(name);
    from.first = name;
    from.second.setLon(lon);
    from.second.setLat(lat);
    //Settings::getInstance()->addHistoryPoint(name, lon, lat);
}

void RouteWindow::targetSet(double lon, double lat, QString name)
{
    ui->toButton->setText(name);
    to.first = name;
    to.second.setLon(lon);
    to.second.setLat(lat);
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
    //Settings::getInstance()->addHistoryPoint(name, lon, lat);
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

void RouteWindow::on_clearThroughButton_clicked()
{
    through.clear();
    ui->throughList->clear();
}
