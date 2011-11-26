#ifndef ROUTINGPROGRESWIDGET_H
#define ROUTINGPROGRESWIDGET_H

#include <QWidget>
#include "tmovableframe.h"

namespace Ui {
    class TRoutingProgressWidget;
}

class TRoutingProgressWidget : public TMovableFrame
{
    Q_OBJECT

public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit TRoutingProgressWidget(QWidget *parent = 0);
    /**
     * @brief
     *
     */
    ~TRoutingProgressWidget();
    /**
     * @brief Function that checks if route is being calculated, and won't show widget if not.
     *
     * @param e - event
     */
    void showEvent(QShowEvent * e);
    /**
     * @brief Indicates start of route calculation and shows widget.
     *
     */
    void startCalculating();
    /**
     * @brief Indicates end of route calculation and hides widget.
     *
     */
    void stopCalculating();
    /**
     * @brief Sets progress of route calculation.
     *
     * @param progress - value of progress
     */
    void setProgress(int progress);

private slots:
    /**
     * @brief TMP!!
     *
     */
    void makeProgress();

private:
    bool calculating;
    Ui::TRoutingProgressWidget *ui;
};

#endif // ROUTINGPROGRESWIDGET_H
