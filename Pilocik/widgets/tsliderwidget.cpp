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

    move(5, 5);
}

TSliderWidget::~TSliderWidget() {
    delete ui;
}

void TSliderWidget::initZoom(int value)
{
    ui->verticalSlider->setValue(value);
}

void TSliderWidget::setMode(TMovableFrame::TMOVABLEMODE mode) {
    /*ui->verticalSlider->setEnabled(!ui->verticalSlider->isEnabled());
    qDebug() << "xxxxxxxxxx";
    frame::setMode(mode);*/
}

void TSliderWidget::on_verticalSlider_valueChanged(int value) {
    if(NavigationWindow::main != NULL)
        NavigationWindow::main->mapRenderer->setZoom(value);
}

void TSliderWidget::on_verticalSlider_sliderPressed() {
    NavigationWindow::main->mapRenderer->setStartZoom(ui->verticalSlider->value());
}

void TSliderWidget::on_verticalSlider_sliderReleased() {
    NavigationWindow::main->mapRenderer->setFinishZoom(ui->verticalSlider->value());
}


void TSliderWidget::modeChanged(TMovableFrame::TMOVABLEMODE& mode){//Tutaj poprawic wg uznania :P przydal by sie inny styl jak sie zmieni na enabled=false
    //ui->verticalSlider->setVisible(mode==TMovableFrame::MOVING?false:true);
    ui->verticalSlider->setEnabled(mode==TMovableFrame::MOVING?false:true);
}

void TSliderWidget::on_plusButton_clicked()
{
    NavigationWindow::main->mapRenderer->setStartZoom(ui->verticalSlider->value());
    initZoom(ui->verticalSlider->value()+10000);
    NavigationWindow::main->mapRenderer->setFinishZoom(ui->verticalSlider->value());
}

void TSliderWidget::on_minusButton_clicked()
{
    NavigationWindow::main->mapRenderer->setStartZoom(ui->verticalSlider->value());
    initZoom(ui->verticalSlider->value()-10000);
    NavigationWindow::main->mapRenderer->setFinishZoom(ui->verticalSlider->value());
}
