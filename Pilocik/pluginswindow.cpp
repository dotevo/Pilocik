#include "pluginswindow.h"
#include "ui_pluginswindow.h"

PluginsWindow::PluginsWindow(NavigationWindow *parent) :
    QFullScreenFrame(parent),
    ui(new Ui::PluginsWindow)
{
    ui->setupUi(this);
    sizeChanged((QWidget*)parent);
}

PluginsWindow::~PluginsWindow()
{
    delete ui;
}

void PluginsWindow::on_okButton_clicked(){
    setVisible(false);
    emit closed();
}
