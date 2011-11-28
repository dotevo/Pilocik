#ifndef TERRORWIDGET_H
#define TERRORWIDGET_H

#include <QWidget>
#include "tmovableframe.h"

namespace Ui {
    class TErrorWidget;
}

class TErrorWidget : public TMovableFrame
{
    Q_OBJECT

public:
    explicit TErrorWidget(QWidget *parent = 0);
    ~TErrorWidget();
    /**
     * @brief Function that checks if there is any message to show, and won't show widget if not.
     *
     * @param e - event
     */
    void showEvent(QShowEvent * e);
    /**
     * @brief Sets message to show.
     *
     * @param message
     */
    void showMessage(QString message);

private slots:
    void on_okButton_clicked();

private:
    bool error;
    Ui::TErrorWidget *ui;
};

#endif // TERRORWIDGET_H
