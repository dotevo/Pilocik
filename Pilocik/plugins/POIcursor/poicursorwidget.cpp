#include "poicursorwidget.h"
#include "ui_poicursorwidget.h"

POIcursorWidget::POIcursorWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::POIcursorWidget)
{
    ui->setupUi(this);
}

POIcursorWidget::~POIcursorWidget()
{
    delete ui;
}
