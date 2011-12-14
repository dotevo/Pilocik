#ifndef UPDATEFRAME_H
#define UPDATEFRAME_H

#include <QFrame>

namespace Ui {
    class UpdateFrame;
}

class UpdateFrame : public QFrame
{
    Q_OBJECT

public:
    explicit UpdateFrame(QWidget *parent = 0);
    ~UpdateFrame();

private:
    Ui::UpdateFrame *ui;
};

#endif // UPDATEFRAME_H
