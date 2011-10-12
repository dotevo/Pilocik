#include "tclockwidget.h"
#include <QGraphicsProxyWidget>
#include "ui_tclockwidget.h"
#include "QDebug"
#include "QTimer"
#include "QDateTime"

TClockWidget::TClockWidget(QGraphicsScene *parent) :
    TMovableFrame(parent),
    ui(new Ui::TClockWidget)
{
    ui->setupUi(this);

    updateTime();
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->start(1000);

    qDebug() << "parent_width " << parent;

    move(50, 25);
}

TClockWidget::~TClockWidget()
{
    delete ui;
}

void TClockWidget::updateTime()
{
    QDateTime time = QDateTime::currentDateTime();
    ui->timeLabel->setText(time.toString("hh:mm:ss, d MMM yyyy"));
    proxy->scene()->update(proxy->rect());
}
