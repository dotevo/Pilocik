#ifndef TMOVABLEFRAME_H
#define TMOVABLEFRAME_H

#include <QFrame>
#include <QGraphicsScene>

class QEvent;
class QMouseEvent;

/**
 * @brief
 *
 * @class TMovableFrame tmovableframe.h "Pilocik/tmovableframe.h"
 */
class TMovableFrame : public QFrame
{
    Q_OBJECT
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
    TMovableFrame(QWidget*parent);

    /**
     * @brief
     *
     * @fn mousePressEvent
     * @param event
     */
    void mousePressEvent(QMouseEvent* event);
    /**
     * @brief
     *
     * @fn mouseMoveEvent
     * @param event
     */
    void mouseMoveEvent(QMouseEvent* event);
    /**
     * @brief
     *
     * @fn mouseReleaseEvent
     * @param event
     */
    void mouseReleaseEvent(QMouseEvent* event);
    /**
     * @brief
     *
     * @fn setMode
     * @param mode
     */
    void setMode(TMovableFrame::TMOVABLEMODE mode);
    /**
     * @brief
     *
     * @fn getMode
     * @return mode
     */    
    TMovableFrame::TMOVABLEMODE getMode();
    virtual void modeChanged(TMovableFrame::TMOVABLEMODE &mode){}
private:
    TMOVABLEMODE mode; /**< TODO */
    QPoint offset; /**< TODO */
};

#endif // TMOVABLEFRAME_H
