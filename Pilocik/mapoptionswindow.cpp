#include "mapoptionswindow.h"
#include "ui_mapoptionswindow.h"

MapOptionsWindow::MapOptionsWindow(NavigationWindow *parent) :
    QFullScreenFrame(parent),
    ui(new Ui::MapOptionsWindow)
{
    ui->setupUi(this);
    //sizeChanged((QWidget*)parent);
}

MapOptionsWindow::~MapOptionsWindow()
{
    delete ui;
}

void MapOptionsWindow::on_okButton_clicked(){
    setVisible(false);
}
