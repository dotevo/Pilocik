#ifndef TMOVABLEFRAME_H
#define TMOVABLEFRAME_H

#include <QFrame>
#include <QGraphicsScene>
#include <QMouseEvent>

class QEvent;
class QMouseEvent;

/**
 * @brief
 *
 * @class TMovableFrame tmovableframe.h "Pilocik/tmovableframe.h"
 */
class TMovableFrame : public QFrame
{
 //   Q_OBJECT
public:
    /**
     * @brief
     *
     * @enum TMOVABLEMODE
     */
    enum TMOVABLEMODE{
        STAND = 0,
        MOVING = 1
    };

    /**
     * @brief
     *
     * @fn TMovableFrame
     * @param scene
     */
    TMovableFrame(QWidget*parent=0):QFrame(parent){}

    /**
     * @brief
     *
     * @fn mousePressEvent
     * @param event
     */
    void mousePressEvent(QMouseEvent* event){
        if(mode==MOVING){
            event->accept(); // do not propagate
            offset = event->pos();
        }
    }

    /**
     * @brief
     *
     * @fn mouseMoveEvent
     * @param event
     */
    void mouseMoveEvent(QMouseEvent* event){
        if(mode==MOVING){
            event->accept(); // do not propagate
            move(mapToParent(event->pos() - offset));
        }
    }

    /**
     * @brief
     *
     * @fn mouseReleaseEvent
     * @param event
     */
    void mouseReleaseEvent(QMouseEvent* event){
        if(mode==MOVING){
            event->accept(); // do not propagate
            offset = QPoint();
        }
    }

    /**
     * @brief
     *
     * @fn setMode
     * @param mode
     */
    void setMode(TMovableFrame::TMOVABLEMODE mode){
        modeChanged(mode);
        this->blockSignals(true);
        this->mode=mode;
    }

    /**
     * @brief
     *
     * @fn getMode
     * @return mode
     */    
    TMovableFrame::TMOVABLEMODE getMode(){
        return mode;
    }

    /**
     * @brief
     *
     * @fn modeChanged
     */
    void modeChanged(TMovableFrame::TMOVABLEMODE &mode){
        Q_UNUSED(mode);
    }
private:
    TMOVABLEMODE mode; /**< TODO */
    QPoint offset; /**< TODO */
};

#endif // TMOVABLEFRAME_H
