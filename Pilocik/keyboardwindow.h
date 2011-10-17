#ifndef KEYBOARDWINDOW_H
#define KEYBOARDWINDOW_H

#include <QFrame>

namespace Ui {
    class KeyboardWindow;
}

class KeyboardWindow : public QFrame
{
    Q_OBJECT

public:
    explicit KeyboardWindow(QWidget *parent = 0);
    ~KeyboardWindow();

private:
    Ui::KeyboardWindow *ui;
};

#endif // KEYBOARDWINDOW_H
