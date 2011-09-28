#include "tclockwidget.h"
#include "ui_tclockwidget.h"

TClockWidget::TClockWidget(QWidget *parent) :
    TWidget(parent),
    ui(new Ui::TClockWidget)
{
    ui->setupUi(this);
}

TClockWidget::~TClockWidget()
{
    delete ui;
}
