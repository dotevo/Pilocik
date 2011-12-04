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
    move(5, 5);

    zoomLevels.append(osmscout::magContinent);
    zoomLevels.append(osmscout::magState);
    zoomLevels.append(osmscout::magStateOver);
    zoomLevels.append(osmscout::magCounty);
    zoomLevels.append(osmscout::magRegion);
    zoomLevels.append(osmscout::magProximity);
    zoomLevels.append(osmscout::magCityOver);
    zoomLevels.append(osmscout::magCity);
    zoomLevels.append(osmscout::magSuburb);
    zoomLevels.append(osmscout::magDetail);
    zoomLevels.append(osmscout::magClose);
    zoomLevels.append(osmscout::magVeryClose);
    zoomLevels.append(osmscout::magStreet);
    zoomLevels.append(osmscout::magBuilding);
}

TSliderWidget::~TSliderWidget() {
    delete ui;
}

void TSliderWidget::initZoom(int value)
{
    ui->verticalSlider->setValue(value);
}

void TSliderWidget::setMode(TMovableFrame::TMOVABLEMODE mode) {
    //ui->verticalSlider->setEnabled(!ui->verticalSlider->isEnabled());
    //qDebug() << "xxxxxxxxxx";
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
    int index = getZoomLevelIndex(NavigationWindow::main->mapRenderer->getZoom());

    if (index + 1 < zoomLevels.size()) {
        int zoom = zoomLevels.at(index + 1);

        if (zoom > 0 && zoom < ui->verticalSlider->maximum()) {
            ui->verticalSlider->setValue(zoom);
            NavigationWindow::main->mapRenderer->setZoom(zoom);
        }
    }
}

void TSliderWidget::on_minusButton_clicked()
{
    int index = getZoomLevelIndex(NavigationWindow::main->mapRenderer->getZoom());

    if (index - 1 >= 0) {
        int zoom = zoomLevels.at(index - 1);

        if (zoom < 0) {
            zoom = 10;
        }

        if (zoom > 0 && zoom < ui->verticalSlider->maximum()) {
            ui->verticalSlider->setValue(zoom);
            NavigationWindow::main->mapRenderer->setZoom(zoom);
        }
    }
}

int TSliderWidget::getZoomLevelIndex(int zoom)
{
    for (int i = 0; i < zoomLevels.size(); i++)
        if (zoomLevels.at(i) >= zoom)
            return i;

    return zoomLevels.size();
}
