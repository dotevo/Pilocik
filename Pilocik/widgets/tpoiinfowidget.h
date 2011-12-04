#ifndef TPOIINFOWIDGET_H
#define TPOIINFOWIDGET_H

#include <QWidget>
#include "tmovableframe.h"
#include "../lib/PiLibocik/include/pilibocik/poi.h"

namespace Ui {
    class TPOIInfoWidget;
}

class TPOIInfoWidget : public TMovableFrame
{
    Q_OBJECT

public:
    explicit TPOIInfoWidget(QWidget *parent = 0);
    ~TPOIInfoWidget();
    /**
     * @brief Function that checks if there is any poi to show, and won't show widget if not.
     *
     * @param e - event
     */
    void showEvent(QShowEvent * e);
    /**
     * @brief Sets poi to show.
     *
     * @param poi
     */
    void showInfo(PiLibocik::Poi poi);

private slots:
    void on_okButton_clicked();

private:
    bool isPOI;
    Ui::TPOIInfoWidget *ui;
};

#endif // TPOIINFOWIDGET_H
