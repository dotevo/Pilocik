#include "tclockwidget.h"
#include "ui_tclockwidget.h"
#include "QDebug"
#include "QTimer"
#include "QDateTime"

TClockWidget::TClockWidget(QWidget *parent) :
    TWidget(parent),
    ui(new Ui::TClockWidget)
{
    ui->setupUi(this);

    updateTime();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->start(1000);

    qDebug() << "parent_width " << parent;

    move(5, 5);
}

TClockWidget::~TClockWidget()
{
    delete ui;
}

void TClockWidget::updateTime()
{
    QDateTime time = QDateTime::currentDateTime();
    ui->timeLabel->setText(time.toString("hh:mm:ss, d MMM yyyy"));
}
