#include "navigationwindow.h"
#include "ui_navigationwindow.h"
#include <QResizeEvent>
#include <QDebug>
#include "twidgetmanager.h"
#include "widgets/tclockwidget.h"

NavigationWindow::NavigationWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NavigationWindow)
{
    ui->setupUi(this);
    TWidgetManager::getInstance().setParent(ui->centralwidget);
    addWidgets();
    TWidgetManager::getInstance().setMode(TMovableFrame::TMOVABLEMODE(TMovableFrame::MOVING));
    ui->menuPanel->setVisible(false);
    ui->routeFrame->setVisible(false);
}

NavigationWindow::~NavigationWindow()
{
    delete ui;
}


void NavigationWindow::addWidgets(){
    TWidgetManager::getInstance().addWidget("Clock", new TClockWidget());
}

void NavigationWindow::resizeEvent ( QResizeEvent * event ){
    //Przesuwanie elementow
    QSize size=this->size();
    QPoint point(0,size.height()-ui->menuButton->geometry().height());
    ui->menuButton->move(point);
    QPoint point2(size.width()-ui->sliderButton->geometry().width(),size.height()-ui->sliderButton->geometry().height());
    ui->sliderButton->move(point2);


    QSize menuPanelsize(size.width(),(size.height()>200)?100:size.height()/2);
    QRect rect=ui->menuPanel->geometry();
    rect.setSize(menuPanelsize);
    ui->menuPanel->setGeometry(rect);
    ui->menuPanel->raise();

    QRect rect2=ui->routeFrame->geometry();
    rect2.setSize(size);
    rect2.setX(0);
    rect2.setY(0);
    ui->routeFrame->setGeometry(rect2);
    ui->routeFrame->raise();


}

void NavigationWindow::on_menuButton_clicked(){
    ui->menuPanel->setVisible(!ui->menuPanel->isVisible());
}

void NavigationWindow::on_routeButton_clicked()
{
    ui->menuPanel->setVisible(false);
    ui->routeFrame->setVisible(true);
}

void NavigationWindow::on_routeBackButton_clicked()
{
    ui->routeFrame->setVisible(false);
}
