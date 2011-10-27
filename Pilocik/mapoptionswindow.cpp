#include "mapoptionswindow.h"
#include "ui_mapoptionswindow.h"
#include "settings.h"

MapOptionsWindow::MapOptionsWindow(NavigationWindow *parent) :
    QFullScreenFrame(parent),
    ui(new Ui::MapOptionsWindow)
{
    ui->setupUi(this);
    //sizeChanged((QWidget*)parent);
    ui->mapFile->setText(Settings::getInstance()->getMapPath());
    ui->mapStylePath->setText(Settings::getInstance()->getMapStylePath());
}

MapOptionsWindow::~MapOptionsWindow()
{
    delete ui;
}


void MapOptionsWindow::on_okButton_clicked(){
    setVisible(false);
    Settings::getInstance()->modifyCoreSettings("mapPath", ui->mapFile->text());
    Settings::getInstance()->modifyCoreSettings("mapStylePath", ui->mapStylePath->text());
}

void MapOptionsWindow::on_backButton_clicked()
{
    setVisible(false);
}
