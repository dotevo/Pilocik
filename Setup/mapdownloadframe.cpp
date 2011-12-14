#include "mapdownloadframe.h"
#include "ui_mapdownloadframe.h"
#include "resourcesmanager.h"
#include "filedownload.h"
#include "mainwindow.h"

#include <QList>
#include <QDebug>
#include <QTimer>
#include <QDir>
#include <QGraphicsEffect>

MapDownloadFrame::MapDownloadFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::MapDownloadFrame)
{
    ui->setupUi(this);
    selectedIdx = -1;
    ui->downloadProgress->hide();
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(0);
    effect->setColor(QColor("#FFFFFF"));
    effect->setOffset(1,1);
    ui->header->setGraphicsEffect(effect);
    ui->mapsList->header()->setResizeMode(1, QHeaderView::Stretch);
}

MapDownloadFrame::~MapDownloadFrame()
{
    delete ui;
}

void MapDownloadFrame::init()
{
    ui->mapsList->clear();
    int idx=0;
    ui->mapsList->header()->hideSection(0);
    foreach(MapResource mr, ResourcesManager::getInstance()->serverMaps)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->mapsList);
        item->setText(0, QString::number(idx++));
        item->setText(1, mr.name);
        item->setText(2, mr.version);
        item->setText(3, QString::number(mr.size/1024/1024)+" MB");
        QString installed;
        QString downloaded;
        foreach(MapResource lmr, ResourcesManager::instance->localMaps)
        {
            if(mr.name == lmr.name)
            {
                downloaded = lmr.version;
                break;
            }
        }
        if(!downloaded.isEmpty())
            item->setForeground(4, QColor("Green"));
        else
            item->setForeground(4, QColor("Red"));
        item->setFont(4, QFont("Trebuchet MS", 9, QFont::Bold));
        item->setText(4, downloaded.isEmpty()?"No":"Yes - ver. "+downloaded);
        foreach(MapResource dmr, ResourcesManager::instance->deviceMaps)
        {
            if(mr.name == dmr.name)
            {
                installed = dmr.version;
                break;
            }
        }
        item->setFont(5, QFont("Trebuchet MS", 9, QFont::Bold));
        if(ActiveSyncComm::getInstance()->getStatus() != 1){
            item->setForeground(5,QColor("Grey"));
            item->setText(5, "Unknown");
        }
        else if(!installed.isEmpty()){
            item->setForeground(5, QColor("Green"));
            item->setText(5, "Yes - ver. "+installed);
        }
        else{
            item->setForeground(5, QColor("Red"));
            item->setText(5, "No");
        }
    }
}

void MapDownloadFrame::on_mapsList_itemSelectionChanged()
{
    if(ui->mapsList->selectedItems().size()==0)
        selectedIdx = -1;
    else
        selectedIdx = ui->mapsList->selectedItems().at(0)->text(0).toInt();
    qDebug()<<selectedIdx;
}

void MapDownloadFrame::on_downloadBtn_clicked()
{
    if(selectedIdx != -1){
        MapResource map = ResourcesManager::instance->serverMaps.at(selectedIdx);
        totalSize = map.size;
        selectedName = map.name;
        typedef QPair<QString,QString> filePair;
        foreach(filePair f, map.files)
            downloadUrls.append("http://194.54.16.66/"+map.path+f.first);
        QTimer::singleShot(0, this, SLOT(nextDownload()));
        QDir mkdir;
        mkdir.mkpath("resources/map/"+selectedName+"/");
        lastSize = 0;
        downloaded = 0;
        ui->downloadProgress->show();
        ui->mapsList->setEnabled(false);
    }
}

void MapDownloadFrame::nextDownload()
{
    downloaded += lastSize;
    if(downloadUrls.size()==0)
    {
        downloadFinished();
        return;
    }
    FileDownload *fd = new FileDownload();
    QString url = downloadUrls.last();
    downloadUrls.removeLast();
    connect(fd, SIGNAL(finished(QString)), this, SLOT(nextDownload()));
    connect(fd, SIGNAL(progress(qint64,qint64)), this, SLOT(progressUpdate(qint64,qint64)));
    fd->doDownload(QUrl(url), "resources/map/"+selectedName+"/");
}

void MapDownloadFrame::progressUpdate(qint64 bytesReceived, qint64 bytesTotal)
{
    lastSize = bytesTotal;
    ui->downloadProgress->setValue(((downloaded+bytesReceived)*100)/totalSize);
}

void MapDownloadFrame::downloadFinished()
{
    ui->downloadProgress->hide();
    ui->mapsList->setEnabled(true);
    ResourcesManager::getInstance()->mapDownloaded(selectedName);
    init();
}

