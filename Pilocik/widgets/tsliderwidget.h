#ifndef TSLIDERWIDGET_H
#define TSLIDERWIDGET_H

#include "../tmovableframe.h"

namespace Ui {
    class TSliderWidget;
}

class TSliderWidget : public TMovableFrame
{
    Q_OBJECT

public:
    explicit TSliderWidget(QWidget *parent = 0);
    ~TSliderWidget();
    typedef TMovableFrame frame;
    void setMode(TMovableFrame::TMOVABLEMODE mode);
    void modeChanged(TMovableFrame::TMOVABLEMODE& mode);

private slots:
    void on_verticalSlider_valueChanged(int value);
    void on_verticalSlider_sliderPressed();
    void on_verticalSlider_sliderReleased();

private:
    Ui::TSliderWidget *ui;
};

#endif // TSLIDERWIDGET_H
