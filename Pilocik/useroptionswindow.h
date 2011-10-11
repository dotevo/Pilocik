#ifndef USEROPTIONSWINDOW_H
#define USEROPTIONSWINDOW_H

#include "qfullscreenframe.h"

namespace Ui {
    class UserOptionsWindow;
}

class UserOptionsWindow : public QFullScreenFrame
{
    Q_OBJECT

public:
    explicit UserOptionsWindow(NavigationWindow *parent);
    ~UserOptionsWindow();

private slots:
    void on_okButton_clicked();

private:
    Ui::UserOptionsWindow *ui;
};

#endif // USEROPTIONSWINDOW_H
