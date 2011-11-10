#include "gpsinfowindow.h"
#include "ui_gpsinfowindow.h"
#include "navigationwindow.h"
#include "gpsreceiver.h"
#include <QString>
#include <QList>


GPSInfoWindow::GPSInfoWindow(NavigationWindow *parent) :
    QFullScreenFrame(parent),
    ui(new Ui::GPSInfoWindow)
{
    ui->setupUi(this);
    sizeChanged((QWidget*)parent);
    clearInfo();
    gps = parent->gps;
    connect(gps, SIGNAL(positionUpdate(GPSdata)), this, SLOT(infoUpdate(GPSdata)));
    connect(gps, SIGNAL(statusUpdate(QString)), this, SLOT(statusUpdate(QString)));
#ifdef Q_OS_WINCE
    ui->pcSimBtn->setVisible(false);
#endif
}

GPSInfoWindow::~GPSInfoWindow()
{
    delete ui;
}

void GPSInfoWindow::on_gpsInfoBackButton_clicked()
{
    setVisible(false);
    emit windowClosed();
}

void GPSInfoWindow::infoUpdate(GPSdata gps_data)
{
    ui->lat->setText(QString::number(gps_data.lat, 'f', 4));
    ui->lon->setText(QString::number(gps_data.lon, 'f', 4));
    ui->speed->setText(QString::number(gps_data.speed, 'f', 0).append(" km/h"));
    ui->angle->setText(QString::number(gps_data.angle, 'f', 1));
    ui->satNo->setText(QString::number(gps_data.tracedSat));
    ui->alt->setText(QString::number(gps_data.alt, 'f', 1).append(" m"));
    QLabel* satLabels []= {ui->sat_label_1, ui->sat_label_2, ui->sat_label_3, ui->sat_label_4, ui->sat_label_5, ui->sat_label_6, ui->sat_label_7, ui->sat_label_8, ui->sat_label_9, ui->sat_label_10, ui->sat_label_11, ui->sat_label_12};
    QProgressBar* satBars []= {ui->snr_1, ui->snr_2, ui->snr_3, ui->snr_4, ui->snr_5, ui->snr_6, ui->snr_7, ui->snr_8, ui->snr_9, ui->snr_10, ui->snr_11, ui->snr_12};
    QStringList satPNR = gps_data.satelitesPRN;
    QList<int> satSNR = gps_data.satelitesSNR;
    for(int i = 0; i < 12; i++)
    {
        if(i<satPNR.size()){
            satLabels[i]->setText(satPNR.at(i));
            satBars[i]->setValue(satSNR.at(i));
        }
        else
        {
            satLabels[i]->setText("--");
            satBars[i]->setValue(0);
        }
    }
    qApp->processEvents();
    gps->clearBuffer();
}

void GPSInfoWindow::clearInfo()
{
    ui->lat->setText("0");
    ui->lon->setText("0");
    ui->speed->setText("0 km/h");
    ui->angle->setText("0");
    ui->satNo->setText("0");
    ui->alt->setText("0 m");
    QLabel* satLabels []= {ui->sat_label_1, ui->sat_label_2, ui->sat_label_3, ui->sat_label_4, ui->sat_label_5, ui->sat_label_6, ui->sat_label_7, ui->sat_label_8, ui->sat_label_9, ui->sat_label_10, ui->sat_label_11, ui->sat_label_12};
    QProgressBar* satBars []= {ui->snr_1, ui->snr_2, ui->snr_3, ui->snr_4, ui->snr_5, ui->snr_6, ui->snr_7, ui->snr_8, ui->snr_9, ui->snr_10, ui->snr_11, ui->snr_12};
    for(int i = 0; i < 12; i++)
    {
        satLabels[i]->setText("--");
        satBars[i]->setValue(0);
    }
}

void GPSInfoWindow::on_startGPSButton_clicked()
{
    gps->setMode(gps->GPS_MODE);
    gps->start();
}

void GPSInfoWindow::on_stopGPSButton_clicked()
{
    gps->disable();
    clearInfo();
}

void GPSInfoWindow::on_pcSimBtn_clicked()
{
    gps->setMode(GPSreceiver::PC_SIMULATION_MODE);
    gps->start();
}

void GPSInfoWindow::statusUpdate(QString status)
{
    ui->status->setText("("+status+")");
}

void GPSInfoWindow::changeEvent(QEvent *e)
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
