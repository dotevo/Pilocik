#ifndef TMOVABLEFRAME_H
#define TMOVABLEFRAME_H

#include <QFrame>
class QEvent;

class TMovableFrame : public QFrame
{
    Q_OBJECT
public:
    TMovableFrame(QWidget *parent = 0);

    bool event ( QEvent * e );
};

#endif // TMOVABLEFRAME_H
