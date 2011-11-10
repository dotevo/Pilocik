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

    void on_languageComboBox_currentIndexChanged(const QString &arg1);

signals:
    void closed();

private:
    Ui::UserOptionsWindow *ui;

    void changeEvent(QEvent *);
};

#endif // USEROPTIONSWINDOW_H
