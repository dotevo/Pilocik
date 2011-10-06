#ifndef USEROPTIONSWINDOW_H
#define USEROPTIONSWINDOW_H

#include <QFrame>

namespace Ui {
    class UserOptionsWindow;
}

class UserOptionsWindow : public QFrame
{
    Q_OBJECT

public:
    explicit UserOptionsWindow(QWidget *parent = 0);
    ~UserOptionsWindow();

private:
    Ui::UserOptionsWindow *ui;
};

#endif // USEROPTIONSWINDOW_H
