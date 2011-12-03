#include "mapdownloadframe.h"
#include "ui_mapdownloadframe.h"

MapDownloadFrame::MapDownloadFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MapDownloadFrame)
{
    ui->setupUi(this);
}

MapDownloadFrame::~MapDownloadFrame()
{
    delete ui;
}
