#include "troutingprogresswidget.h"
#include "ui_troutingprogresswidget.h"
#include "QDebug"
#include "QTimer"
#include "QDateTime"
#include "widgets/terrorwidget.h"
#include "twidgetmanager.h"

TRoutingProgressWidget::TRoutingProgressWidget(QWidget *parent) :
    TMovableFrame(parent),
    ui(new Ui::TRoutingProgressWidget)
{
    ui->setupUi(this);

    calculating = false;
}

TRoutingProgressWidget::~TRoutingProgressWidget()
{
    delete ui;
}

void TRoutingProgressWidget::startCalculating()
{
    ui->progressBar->setValue(0);
    calculating = true;
    setVisible(true);
}

void TRoutingProgressWidget::stopCalculating()
{
    calculating = false;
    setVisible(false);
}

void TRoutingProgressWidget::setProgress(int progress)
{
    if(progress >= 0 && progress < 100) {
        ui->progressBar->setValue(progress);
    } else if(progress >= 100){
        stopCalculating();
    }
}

void TRoutingProgressWidget::showEvent(QShowEvent * e)
{
    if(!calculating) {
        setVisible(false);
    }
}
