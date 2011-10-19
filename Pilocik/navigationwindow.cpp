#include "navigationwindow.h"
#include "ui_navigationwindow.h"
#include <QResizeEvent>
#include <QDebug>
#include "twidgetmanager.h"
#include "widgets/tclockwidget.h"
#include "routewindow.h"
#include "optionswindow.h"

NavigationWindow *NavigationWindow::main=0;

NavigationWindow::NavigationWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NavigationWindow)
{
    ui->setupUi(this);
    TWidgetManager::getInstance()->setParent(this);
    addWidgets();
    TWidgetManager::getInstance()->setMode(TMovableFrame::TMOVABLEMODE(TMovableFrame::MOVING));
    ui->menuPanel->setVisible(false);
    addFrames();
    ui->widget->setVisible(true);
    ui->widget->lower();
}

NavigationWindow::~NavigationWindow()
{
    delete ui;
    delete routeWin;
    delete optionsWin;
    delete TWidgetManager::getInstance();
}

void NavigationWindow::addWidgets(){
    TWidgetManager::getInstance()->addWidget("Clock", new TClockWidget(this));
}

void NavigationWindow::addFrames(){
    routeWin=new RouteWindow(this);
    routeWin->setVisible(false);
    optionsWin=new OptionsWindow(this);
    optionsWin->setVisible(false);
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

    emit sizeChanged(this);
}

void NavigationWindow::on_menuButton_clicked(){
    ui->menuPanel->setVisible(!ui->menuPanel->isVisible());
}

void NavigationWindow::on_routeButton_clicked()
{
    ui->menuPanel->setVisible(false);
    routeWin->setVisible(true);
    routeWin->raise();
}


void NavigationWindow::on_optionsButton_clicked()
{
    ui->menuPanel->setVisible(false);
    optionsWin->setVisible(true);
    optionsWin->raise();
}

void NavigationWindow::on_zoomSlider_valueChanged(int value)
{
    ui->widget->setZoom(value);
}

void NavigationWindow::on_zoomSlider_sliderPressed()
{
    ui->widget->setStartZoom(ui->zoomSlider->value());
}

void NavigationWindow::on_zoomSlider_sliderReleased()
{
    ui->widget->setFinishZoom(ui->zoomSlider->value());
}

void NavigationWindow::on_pushButton_4_clicked()
{
    this->close();
}

