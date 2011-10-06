#include "mapoptionswindow.h"
#include "ui_mapoptionswindow.h"

MapOptionsWindow::MapOptionsWindow(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MapOptionsWindow)
{
    ui->setupUi(this);
}

MapOptionsWindow::~MapOptionsWindow()
{
    delete ui;
}
