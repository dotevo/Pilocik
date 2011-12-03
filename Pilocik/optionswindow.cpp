#include "optionswindow.h"
#include "ui_optionswindow.h"
#include "mapoptionswindow.h"
#include "gpsoptionswindow.h"
#include "useroptionswindow.h"
#include "creditswindow.h"
#include "navigationwindow.h"
#include "pluginswindow.h"

OptionsWindow::OptionsWindow(NavigationWindow *parent) :
    QFullScreenFrame(parent),
    ui(new Ui::OptionsWindow)
{
    ui->setupUi(this);
    sizeChanged((QWidget*)parent);

    moWin=new MapOptionsWindow(parent);
    moWin->setVisible(false);

    gpsWin=new GPSOptionsWindow(parent);
    gpsWin->setVisible(false);

    userWin=new UserOptionsWindow(parent);
    userWin->setVisible(false);

    creditsWin=new CreditsWindow(parent);
    creditsWin->setVisible(false);

    pluginsWin=new PluginsWindow(parent);
    pluginsWin->setVisible(false);
}

OptionsWindow::~OptionsWindow() {
    delete ui;
    delete moWin;
    delete gpsWin;
    delete userWin;
}

void OptionsWindow::on_backButton_clicked() {
    setVisible(false);
    emit closed();
}

void OptionsWindow::on_mapButton_clicked() {
    moWin->setVisible(true);
    moWin->raise();
    moWin->setStartFocus();
    connect(moWin, SIGNAL(closed()),
            this, SLOT(optionWindowClosed()));
    setVisible(false);

}

void OptionsWindow::on_GPSButton_clicked() {
    gpsWin->setVisible(true);
    gpsWin->raise();
    gpsWin->setStartFocus();
    connect(gpsWin, SIGNAL(closed()),
            this, SLOT(optionWindowClosed()));
    setVisible(false);
}

void OptionsWindow::on_userButton_clicked() {
    userWin->setVisible(true);
    userWin->raise();
    connect(userWin, SIGNAL(closed()),
            this, SLOT(optionWindowClosed()));
    setVisible(false);
}

void OptionsWindow::on_creditsButton_clicked()
{
    creditsWin->setVisible(true);
    creditsWin->raise();
    connect(creditsWin, SIGNAL(closed()),
            this, SLOT(optionWindowClosed()));
    setVisible(false);
}

void OptionsWindow::optionWindowClosed() {
    setVisible(true);
}

void OptionsWindow::changeEvent(QEvent *e)
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

void OptionsWindow::on_pluginsButton_clicked(){
    pluginsWin->setVisible(true);
    pluginsWin->raise();
    connect(pluginsWin, SIGNAL(closed()),
            this, SLOT(optionWindowClosed()));
    setVisible(false);
}
