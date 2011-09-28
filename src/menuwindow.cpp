#include "menuwindow.h"
#include "ui_menuwindow.h"
#include "twidgetmanager.h"
#include <QLayout>

//Widgets
#include "widgets/tclockwidget.h"


MenuWindow::MenuWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MenuWindow)
{
    ui->setupUi(this);

    TWidgetManager::getInstance().setParent(ui->centralWidget);
    // Tomek: IMO should be one button that changes the mode, so this is temporary...
    TWidgetManager::getInstance().setMode(TMovableFrame::TMOVABLEMODE(TMovableFrame::STAND));
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_4->setEnabled(false);

    addWidgets();
}

void MenuWindow::addWidgets(){
    TWidgetManager::getInstance().addWidget("Clock", new TClockWidget());
}

MenuWindow::~MenuWindow()
{
    delete ui;
}

void MenuWindow::on_pushButton_3_clicked()
{
    TWidgetManager::getInstance().setMode(TMovableFrame::TMOVABLEMODE(TMovableFrame::MOVING));
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(true);
}

void MenuWindow::on_pushButton_4_clicked()
{
    TWidgetManager::getInstance().setMode(TMovableFrame::TMOVABLEMODE(TMovableFrame::STAND));
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_4->setEnabled(false);
}
