#ifndef INSTALLFRAME_H
#define INSTALLFRAME_H

#include <QFrame>

namespace Ui {
    class InstallFrame;
}

class InstallFrame : public QFrame
{
    Q_OBJECT

public:
    explicit InstallFrame(QWidget *parent = 0);
    ~InstallFrame();

private:
    Ui::InstallFrame *ui;
};

#endif // INSTALLFRAME_H
