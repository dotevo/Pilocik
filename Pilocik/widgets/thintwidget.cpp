#include "thintwidget.h"
#include "ui_thintwidget.h"

THintWidget::THintWidget(QWidget *parent) :
    TMovableFrame(parent),
    ui(new Ui::THintWidget)
{
    ui->setupUi(this);
}

THintWidget::~THintWidget()
{
    delete ui;
}

void THintWidget::setPre(QString pre)
{
    ui->labelPre->setText(pre);
}

void THintWidget::setDistance(QString distance)
{
    ui->labelDist->setText(distance);
}

void THintWidget::setHintContent(QString content)
{
    ui->labelHint->setText(content);
}

QString THintWidget::getPre()
{
    return ui->labelPre->text();
}

QString THintWidget::getDistance()
{
    return ui->labelDist->text();
}

QString THintWidget::getHintContent()
{
    return ui->labelHint->text();
}

void THintWidget::updateAll(QString pre, QString distance, QString content)
{
    setPre(pre);
    setDistance(distance);
    setHintContent(content);
}
