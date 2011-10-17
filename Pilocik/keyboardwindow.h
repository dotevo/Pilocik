#ifndef KEYBOARDWINDOW_H
#define KEYBOARDWINDOW_H

#include "qfullscreenframe.h"
#include <QSignalMapper>
#include <QLineEdit>

namespace Ui {
    class KeyboardWindow;
}

class KeyboardWindow : public QFullScreenFrame
{
    Q_OBJECT
public:
    explicit KeyboardWindow(NavigationWindow *parent,QLineEdit *text,QWidget *view);
    ~KeyboardWindow();
    static bool opened;
private:
    Ui::KeyboardWindow *ui;
    QWidget *lastFocusedWidget;
    QSignalMapper signalMapper;
    QLineEdit *textEdit;
protected:
    bool event(QEvent *e);
private slots:
    void saveFocusWidget(QWidget *oldFocus, QWidget *newFocus);
    void buttonClicked(QWidget *w);
    void on_closeButton_clicked();
    void on_space_button_clicked();

    void on_back_button_clicked();

signals:
    void close(KeyboardWindow*win);
};

#endif // KEYBOARDWINDOW_H
