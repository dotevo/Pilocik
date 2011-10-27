#ifndef TSLIDERWIDGET_H
#define TSLIDERWIDGET_H

#include "../TMovableFrame.h"

namespace Ui {
    class TSliderWidget;
}

class TSliderWidget : public TMovableFrame
{
    Q_OBJECT

public:
    explicit TSliderWidget(QWidget *parent = 0);
    ~TSliderWidget();

private slots:
    void on_verticalSlider_valueChanged(int value);

    void on_verticalSlider_sliderPressed();

    void on_verticalSlider_sliderReleased();

private:
    Ui::TSliderWidget *ui;
};

#endif // TSLIDERWIDGET_H
