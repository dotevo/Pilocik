#include "menuwindow.h"
#include "ui_menuwindow.h"

MenuWindow::MenuWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MenuWindow)
{
    ui->setupUi(this);
}

MenuWindow::~MenuWindow()
{
    delete ui;
}

void MenuWindow::on_pushButton_clicked()
{
}

void MenuWindow::on_pushButton_3_clicked()
{
    ui->frame->setMode(TMovableFrame::TMOVABLEMODE(TMovableFrame::MOVING) );
    ui->frame_2->setMode(TMovableFrame::TMOVABLEMODE(TMovableFrame::MOVING) );
}

void MenuWindow::on_pushButton_4_clicked()
{

    ui->frame->setMode(TMovableFrame::TMOVABLEMODE(TMovableFrame::STAND) );
    ui->frame_2->setMode(TMovableFrame::TMOVABLEMODE(TMovableFrame::STAND) );
}
