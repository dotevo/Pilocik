#include "mapoptionswindow.h"
#include "ui_mapoptionswindow.h"
#include "settings.h"
#include <pilibocik/poifileppoi.h>
#include <QDebug>

MapOptionsWindow::MapOptionsWindow(NavigationWindow *parent) :
    QFullScreenFrame(parent),
    ui(new Ui::MapOptionsWindow)
{
    ui->setupUi(this);
    //sizeChanged((QWidget*)parent);
    ui->mapFile->setText(Settings::getInstance()->getMapPath());
    ui->mapStylePath->setText(Settings::getInstance()->getMapStylePath());

    poiDisplaySettings = Settings::getInstance()->getPoiDisplaySettings();
    PiLibocik::PoiFilePPOI poiDatabase;
    poiTypes = poiDatabase.loadPOIsTypesFromFile(Settings::getInstance()->getPoiFilePath());

    ui->poiLayout->setColumnStretch(1,10);
    ui->poiLayout->setColumnStretch(3,10);
    ui->poiLayout->setColumnStretch(5,10);
    ui->poiLayout->setColumnStretch(7,10);
    int i=0;
    foreach(PiLibocik::PoiDisplay poi, poiDisplaySettings)
    {
        QCheckBox* cb = new QCheckBox(poiTypes[poi.getType()]);
        cb->setChecked(poi.getDisplay());
        QPixmap image(Settings::getInstance()->getPoiIconsDir()+poiDisplaySettings[i].getIconPath());
        QLabel* label = new QLabel();
        label->setPixmap(image);
        ui->poiLayout->addWidget(label, i*2/8, i*2%8);
        ui->poiLayout->addWidget(cb, (i*2+1)/8, (i*2+1)%8);
        i++;
    }
}

MapOptionsWindow::~MapOptionsWindow()
{
    delete ui;
}

void MapOptionsWindow::on_okButton_clicked(){
    setVisible(false);
    Settings::getInstance()->modifyCoreSettings("mapPath", ui->mapFile->text());
    Settings::getInstance()->modifyCoreSettings("mapStylePath", ui->mapStylePath->text());
    QList<QCheckBox *> cbs = ui->poiSettings->findChildren<QCheckBox *>();
    foreach(QCheckBox *cb, cbs)
        poiDisplaySettings[poiTypes.key(cb->text())].setDisplay(cb->isChecked());

    Settings::getInstance()->modifyPoiDisplaySettings(poiDisplaySettings);
    emit closed();
}

void MapOptionsWindow::on_backButton_clicked()
{
    setVisible(false);
    emit closed();
}

void MapOptionsWindow::setStartFocus()
{
    ui->mapOptionsLabel->setFocus();
}

void MapOptionsWindow::changeEvent(QEvent *e)
{
    QFrame::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
