#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MenuWindow;
}

class MenuWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MenuWindow(QWidget *parent = 0);
    ~MenuWindow();

private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

private:
    Ui::MenuWindow *ui;
    void addWidgets();
};

#endif // MENUWINDOW_H
