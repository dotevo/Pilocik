#ifndef TMOVABLEFRAME_H
#define TMOVABLEFRAME_H

#include <QFrame>
class QEvent;
class QMouseEvent;

class TMovableFrame : public QFrame
{
    Q_OBJECT
public:
    enum TMOVABLEMODE{
        STAND = 0,
        MOVING = 1
    };

    TMovableFrame(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void setMode(TMovableFrame::TMOVABLEMODE mode);
private:
    TMOVABLEMODE mode;
    QPoint offset;
};

#endif // TMOVABLEFRAME_H
