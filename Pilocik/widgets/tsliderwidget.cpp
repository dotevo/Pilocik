#include "tsliderwidget.h"
#include "ui_tsliderwidget.h"
#include "../navigationwindow.h"
#include "../settings.h"

#include <QDebug>

TSliderWidget::TSliderWidget(QWidget *parent) :
    TMovableFrame(parent),
    ui(new Ui::TSliderWidget)
{
    ui->setupUi(this);

    Settings* settings = Settings::getInstance();
    //ui->verticalSlider->setValue(settings->getZoom());

    move(5, 5);
}

TSliderWidget::~TSliderWidget() {
    delete ui;
}

void TSliderWidget::setMode(TMovableFrame::TMOVABLEMODE mode) {
    /*ui->verticalSlider->setEnabled(!ui->verticalSlider->isEnabled());
    qDebug() << "xxxxxxxxxx";
    frame::setMode(mode);*/
}

void TSliderWidget::on_verticalSlider_valueChanged(int value) {
    //NavigationWindow::main->mapRenderer->setZoom(value);
}

void TSliderWidget::on_verticalSlider_sliderPressed() {
    //NavigationWindow::main->mapRenderer->setStartZoom(ui->verticalSlider->value());
}

void TSliderWidget::on_verticalSlider_sliderReleased() {
    //NavigationWindow::main->mapRenderer->setFinishZoom(ui->verticalSlider->value());
}


void TSliderWidget::modeChanged(TMovableFrame::TMOVABLEMODE& mode){//Tutaj poprawic wg uznania :P przydal by sie inny styl jak sie zmieni na enabled=false
    //ui->verticalSlider->setVisible(mode==TMovableFrame::MOVING?false:true);
    ui->verticalSlider->setEnabled(mode==TMovableFrame::MOVING?false:true);
}
