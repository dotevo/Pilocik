#ifndef TSLIDERWIDGET_H
#define TSLIDERWIDGET_H

#include "../tmovableframe.h"

#include <QList>

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
    void initZoom(int value);
    void setMode(TMovableFrame::TMOVABLEMODE mode);
    void modeChanged(TMovableFrame::TMOVABLEMODE& mode);

private slots:
    void on_verticalSlider_valueChanged(int value);
    void on_verticalSlider_sliderPressed();
    void on_verticalSlider_sliderReleased();

    void on_plusButton_clicked();

    void on_minusButton_clicked();

private:
    Ui::TSliderWidget *ui;

    QList<int> zoomLevels;
    int getZoomLevelIndex(int zoom);
};

#endif // TSLIDERWIDGET_H
