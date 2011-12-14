#include "pluginsframe.h"
#include "ui_pluginsframe.h"
#include "resourcesmanager.h"
#include "filedownload.h"
#include "activesynccomm.h"

#include <QGraphicsDropShadowEffect>

PluginsFrame::PluginsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PluginsFrame)
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

void PluginsFrame::init()
{
    ui->stylesList->clear();
    int idx=0;
    foreach(FileResource sr, ResourcesManager::getInstance()->serverPlugins)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->stylesList);
        item->setText(0, QString::number(idx++));
        item->setText(1, sr.name);
        item->setText(2, sr.version);
        QString installed;
        foreach(FileResource dsr, ResourcesManager::instance->devicePlugins)
        {
            if(sr.name == dsr.name)
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

void PluginsFrame::on_stylesList_itemSelectionChanged()
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

void PluginsFrame::on_installBtn_clicked()
{
    if(selectedIdx != -1){
        FileResource pulgin = ResourcesManager::instance->serverPlugins.at(selectedIdx);
        selectedName = pulgin.name;
        selectedFile = pulgin.file;
        FileDownload *fd = new FileDownload();
        connect(fd, SIGNAL(finished(QString)), this, SLOT(downloadFinished()));
        ui->progressBar->show();
        fd->doDownload(QUrl("http://194.54.16.66/plugins/"+pulgin.file), "resources/plugins/");
    }
}

void PluginsFrame::downloadFinished()
{
    ResourcesManager *rm = ResourcesManager::getInstance();
    rm->pluginDownloaded(selectedName);
    ui->progressBar->setValue(50);
    ActiveSyncComm *asc = ActiveSyncComm::getInstance();
    asc->copyToDevice("resources/plugins/"+selectedFile, "/ResidentFlash/Pilocik2/plugins/"+selectedFile);
    Sleep(100);
    ui->progressBar->setValue(100);
    Sleep(10);
    ui->progressBar->hide();
    ui->progressBar->setValue(0);
    rm->pluginDeviceInstall(selectedName, "/ResidentFlash/Pilocik.xml");
    ui->status->setText("Plugin \""+selectedName+"\" installed successfuly.");
}

PluginsFrame::~PluginsFrame()
{
    delete ui;
}
