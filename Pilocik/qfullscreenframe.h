#ifndef QFULLSCREENFRAME_H
#define QFULLSCREENFRAME_H

#include <QFrame>
class NavigationWindow;
class QFullScreenFrame : public QFrame
{
    Q_OBJECT
public:
    explicit QFullScreenFrame(NavigationWindow *parent);
public slots:    
    void sizeChanged(QWidget *size);

};

#endif // QFULLSCREENFRAME_H
