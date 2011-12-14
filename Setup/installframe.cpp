#include "installframe.h"
#include "ui_installframe.h"
#include "activesynccomm.h"
#include "installer.h"

#include <QFile>
#include <QTextStream>
#include <QGraphicsDropShadowEffect>

InstallFrame::InstallFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::InstallFrame)
{
    ui->setupUi(this);
    ui->pbFrame->hide();
    ui->pathFrame->hide();
    ui->mapFrame->hide();
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(0);
    effect->setColor(QColor("#FFFFFF"));
    effect->setOffset(1,1);
    ui->header->setGraphicsEffect(effect);
    ui->mapsList->header()->setResizeMode(1, QHeaderView::Stretch);
    ui->errorFrame->setVisible(false);

}

InstallFrame::~InstallFrame()
{
    delete ui;
}

void InstallFrame::deviceStatus(int status)
{
    if(status == 1)
    {
        ui->errorFrame->setVisible(false);
        if(installMode == WINCEactivesync){
            ui->pathFrame->setVisible(true);
            ui->mapFrame->setVisible(true);
        }
    }
}

void InstallFrame::init()
{
    ui->mapsList->clear();
    int idx=0;
    ui->mapsList->header()->hideSection(0);
    ui->mapsList->header()->setResizeMode(1, QHeaderView::Stretch);
    foreach(MapResource mr, ResourcesManager::getInstance()->localMaps)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->mapsList);
        item->setText(0, QString::number(idx++));
        item->setText(1, mr.name);
        item->setText(2, mr.version);
    }
}

void InstallFrame::on_mapsList_itemSelectionChanged()
{
    if(ui->mapsList->selectedItems().size()==0)
        maps.clear();
    else{
        foreach(QTreeWidgetItem *el, ui->mapsList->selectedItems()){
            maps.append(ResourcesManager::getInstance()->localMaps.at(el->text(0).toInt()));
        }
    }
}

void InstallFrame::on_installBtn_clicked()
{
    if(!maps.isEmpty())
    {
        Installer *inst = new Installer();
        inst->init(ui->installPath->text(), maps);
        ui->pathFrame->hide();
        ui->mapFrame->hide();
        ui->selectFrame->hide();
        ui->pbFrame->show();
        connect(inst, SIGNAL(progressUpdate(int)), ui->progressBar, SLOT(setValue(int)));
        //inst.install();
        inst->start();
    }
}

void InstallFrame::on_PCbtn_clicked()
{
    installMode = PCharddrive;
}

void InstallFrame::on_winCEbtn_clicked()
{
    installMode = WINCEactivesync;
    if(ActiveSyncComm::getInstance()->getStatus()!=1)
    {
        ui->errorFrame->setVisible(true);
        connect(ActiveSyncComm::getInstance(), SIGNAL(statusUpdate(int)), this, SLOT(deviceStatus(int)));
    } else {
        ui->pathFrame->show();
        ui->mapFrame->show();
    }
}



