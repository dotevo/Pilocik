#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow* main;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QList<QPushButton *> tabs;
    int activeTab;
    QPoint dragPosition;
    bool isDraging;

    void selectTab(int id);

private slots:
    void showEvent(QShowEvent *event);
    void on_tab1_clicked();
    void on_tab2_clicked();
    void on_tab3_clicked();
    void on_tab4_clicked();
    void on_tab5_clicked();
    void on_tab6_clicked();
    void on_exitBtn_clicked();
    void on_minimalizeBtn_clicked();
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // MAINWINDOW_H
