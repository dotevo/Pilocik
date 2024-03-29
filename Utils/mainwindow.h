#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "databasegen.h"
#include "gpsemulator.h"
#include <osmscout/Partitioning.h>

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
    osmscout::Partitioning *part;
    QPoint dragPosition;
    bool isDraging;
    short currentTab;

    void updateTab();

private slots:
    void calculationFinished();
    void on_genBtn_clicked();
    void on_simStopBtn_clicked();
    void on_simStartBtn_clicked();
    void on_simPauseBtn_clicked();
    void on_dataInitOutputFilePathButton_clicked();
    void on_dataInitMapPathButton_clicked();
    void on_prioCalcDataFilePathButton_clicked();
    void on_prioCalcOutputFilePathButton_clicked();
    void on_partCalcDataFilePathButton_clicked();
    void on_partCalcPrioFilePathButton_clicked();
    void on_partCalcOutputPathButton_clicked();
    void on_dataInitBtn_clicked();
    void on_osmPathButton_clicked();
    void on_outputDirButton_clicked();
    void on_simPathButton_clicked();
    void on_cancelBtn_clicked();
    void on_poiFilePathBrowseButton_clicked();
    void on_prioCalcButton_clicked();
    void on_partCalcButton_clicked();
    void on_partBinaryRenderButton_clicked();
    void on_partitionRenderButton_clicked();
    void on_hideButton_clicked();
    void on_closeButton_clicked();
    void on_tabMapGenButton_clicked();
    void on_tabSimButton_clicked();
    void on_tabPartGenButton_clicked();
    void on_tabPartRenderButton_clicked();
    void on_poiGenerateButton_clicked();

    void on_poiConfigBrowseButton_clicked();

    void on_poiMapPathBrowseButton_clicked();

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // MAINWINDOW_H
