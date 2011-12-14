#include "updateframe.h"
#include "ui_updateframe.h"
#include "resourcesmanager.h"

#include <QGraphicsDropShadowEffect>

UpdateFrame::UpdateFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::UpdateFrame)
{
    ui->setupUi(this);
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(0);
    effect->setColor(QColor("#FFFFFF"));
    effect->setOffset(1,1);
    ui->header->setGraphicsEffect(effect);
    ui->stylesList->header()->hideSection(0);
    ui->stylesList->header()->setResizeMode(1, QHeaderView::Stretch);
    ui->stylesList->header()->setResizeMode(2, QHeaderView::Stretch);
    ui->progressBar->hide();
}

UpdateFrame::~UpdateFrame()
{
    delete ui;
}
