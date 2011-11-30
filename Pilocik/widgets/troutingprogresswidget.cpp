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

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(makeProgress()));
    timer->start(1000);

    calculating = false;
}

TRoutingProgressWidget::~TRoutingProgressWidget()
{
    delete ui;
}

void TRoutingProgressWidget::startCalculating(PiLibocik::Position startingPosition, PiLibocik::Position endPosition)
{
    calculating = true;
    ui->progressBar->setValue(0);
    //setVisible(true);
}

void TRoutingProgressWidget::stopCalculating()
{
    calculating = false;
    setVisible(false);
}

void TRoutingProgressWidget::setProgress(int progress)
{
    if(progress >= 0 && progress <= 100) {
        ui->progressBar->setValue(progress);
    } else {
        // ERROR
    }
}

void TRoutingProgressWidget::showEvent(QShowEvent * e)
{
    if(!calculating) {
        setVisible(false);
    }
}

void TRoutingProgressWidget::makeProgress()
{
    int progr = ui->progressBar->value()+15;
    if(progr > 99 && calculating == true) {
        calculating = false;
        setVisible(false);
        ((TErrorWidget*) TWidgetManager::getInstance()->getWidget("ErrorMessage"))->showMessage(QString("Unable to calculate route!"));
    } else {
        setProgress(progr%100);
    }
}
