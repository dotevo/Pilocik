#include "stylesframe.h"
#include "ui_stylesframe.h"
#include "resourcesmanager.h"
#include "filedownload.h"
#include "activesynccomm.h"

#include <QGraphicsDropShadowEffect>
#include <QDebug>

StylesFrame::StylesFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::StylesFrame)
{
    ui->setupUi(this);
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(0);
    effect->setColor(QColor("#FFFFFF"));
    effect->setOffset(1,1);
    ui->header->setGraphicsEffect(effect);
    ui->stylesList->header()->hideSection(0);
    ui->stylesList->header()->setResizeMode(1, QHeaderView::Stretch);
    ui->progressBar->hide();

    selectedIdx = -1;
}

void StylesFrame::init()
{
    ui->stylesList->clear();
    int idx=0;
    foreach(FileResource fr, ResourcesManager::getInstance()->serverStyles)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->stylesList);
        item->setText(0, QString::number(idx++));
        item->setText(1, fr.name);
        item->setText(2, fr.version);
        QString installed;
        foreach(FileResource dsr, ResourcesManager::instance->deviceStyles)
        {
            if(fr.name == dsr.name)
            {
                installed = dsr.version;
                break;
            }
        }
        item->setFont(3, QFont("Trebuchet MS", 9, QFont::Bold));
        if(ActiveSyncComm::getInstance()->getStatus() != 1){
            item->setForeground(3, QColor("Grey"));
            item->setText(3, "Unknown");
        }
        else if(!installed.isEmpty()){
            item->setForeground(3, QColor("Green"));
            item->setText(3,"Yes - ver. "+installed);
        }
        else{
            item->setForeground(3, QColor("Red"));
            item->setText(3, "No");
        }
    }
}

void StylesFrame::on_stylesList_itemSelectionChanged()
{
    if(ui->stylesList->selectedItems().size()==0)
        selectedIdx = -1;
    else
        selectedIdx = ui->stylesList->selectedItems().at(0)->text(0).toInt();
    if(selectedIdx != -1){
        if(ui->stylesList->selectedItems().at(0)->text(3).contains("Yes"))
            ui->installBtn->setEnabled(false);
        else
            ui->installBtn->setEnabled(true);
    }
}

void StylesFrame::on_installBtn_clicked()
{
    if(selectedIdx != -1){
        FileResource style = ResourcesManager::instance->serverStyles.at(selectedIdx);
        selectedName = style.name;
        selectedFile = style.file;
        selectedVersion = style.version;
        ui->progressBar->show();
        FileDownload *fd = new FileDownload();
        connect(fd, SIGNAL(finished(QString)), this, SLOT(downloadFinished()));
        fd->doDownload(QUrl("http://194.54.16.66/styles/"+style.file), "resources/styles/");
    }
}

void StylesFrame::downloadFinished()
{
    bool haveLocal = false;
    foreach(FileResource fr, ResourcesManager::instance->localStyles)
    {
        if(fr.name == selectedName && fr.version == selectedVersion){
            haveLocal = true;
            break;
        }
    }
    ResourcesManager *rm = ResourcesManager::getInstance();
    if(!haveLocal)
        rm->styleDownloaded(selectedName);
    ui->progressBar->setValue(50);
    ActiveSyncComm *asc = ActiveSyncComm::getInstance();
    qDebug()<<"resources/styles/"+selectedName;
    asc->copyToDevice("resources/styles/"+selectedFile, "/ResidentFlash/Pilocik2/styles/"+selectedFile);
    Sleep(100);
    ui->progressBar->setValue(100);
    Sleep(10);
    ui->progressBar->hide();
    ui->progressBar->setValue(0);
    rm->styleDeviceInstall(selectedName, "/ResidentFlash/Pilocik.xml");
    ui->status->setText("Style \""+selectedName+"\" installed successfuly.");
}

StylesFrame::~StylesFrame()
{
    delete ui;
}
