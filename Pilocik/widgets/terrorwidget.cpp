#include "terrorwidget.h"
#include "ui_terrorwidget.h"

TErrorWidget::TErrorWidget(QWidget *parent) :
    TMovableFrame(parent),
    ui(new Ui::TErrorWidget)
{
    ui->setupUi(this);

    error = false;
}

TErrorWidget::~TErrorWidget()
{
    delete ui;
}

void TErrorWidget::showEvent(QShowEvent * e)
{
    if(!error) {
        setVisible(false);
    }
}

void TErrorWidget::showMessage(QString message)
{
    error = true;
    ui->errorMessage->setText(message);
    setVisible(true);
}

void TErrorWidget::on_okButton_clicked()
{
    error = false;
    setVisible(false);
}
