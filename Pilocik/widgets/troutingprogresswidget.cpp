#include "troutingprogresswidget.h"
#include "ui_troutingprogresswidget.h"
#include "QDebug"
#include "QTimer"
#include "QDateTime"

TRoutingProgressWidget::TRoutingProgressWidget(QWidget *parent) :
    TMovableFrame(parent),
    ui(new Ui::TRoutingProgressWidget)
{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(makeProgress()));
    timer->start(1000);

    calculating = false;
}

TRoutingProgressWidget::~TRoutingProgressWidget()
{
    delete ui;
}

void TRoutingProgressWidget::startCalculating()
{
    calculating = true;
    ui->progressBar->setValue(0);
    setVisible(true);
}

void TRoutingProgressWidget::stopCalculating()
{
    calculating = false;
    setVisible(false);
}

void TRoutingProgressWidget::setProgress(int progress)
{
    ui->progressBar->setValue(progress);
}

void TRoutingProgressWidget::showEvent(QShowEvent * e)
{
    if(!calculating) {
        setVisible(false);
    }
}

void TRoutingProgressWidget::makeProgress()
{
    setProgress((ui->progressBar->value()+1)%100);
}
