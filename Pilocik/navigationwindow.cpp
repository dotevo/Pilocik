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
    QSize menuPanelsize(size.width(),size.height()/2);
    QRect rect=ui->menuPanel->geometry();
    rect.setSize(menuPanelsize);
    ui->menuPanel->setGeometry(rect);
    ui->menuPanel->raise();
}

void NavigationWindow::on_menuButton_clicked(){
    ui->menuPanel->setVisible(!ui->menuPanel->isVisible());
}
