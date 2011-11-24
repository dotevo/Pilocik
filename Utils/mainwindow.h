#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "databasegen.h"
#include "gpsemulator.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    DatabaseGen *gen;
    GPSemulator *gps;


private slots:
    void on_genBtn_clicked();
    void on_partitionOk_clicked();
    void on_simStopBtn_clicked();
    void on_simStartBtn_clicked();
    void on_simPauseBtn_clicked();
    void on_dataInitOutputFilePathButton_clicked();
    void on_dataInitMapPathButton_clicked();
};

#endif // MAINWINDOW_H
