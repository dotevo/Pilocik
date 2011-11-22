#ifndef THINTWIDGET_H
#define THINTWIDGET_H

#include "../tmovableframe.h"
#include <QWidget>

namespace Ui {
    class THintWidget;
}

class THintWidget : public TMovableFrame
{
    Q_OBJECT

public:
    explicit THintWidget(QWidget *parent = 0);
    ~THintWidget();

    void setPre(QString pre);
    void setDistance(QString distance);
    void setHintContent(QString content);

    QString getPre();
    QString getDistance();
    QString getHintContent();

    void updateAll(QString pre, QString distance, QString content);

private:
    Ui::THintWidget *ui;
};

#endif // THINTWIDGET_H
