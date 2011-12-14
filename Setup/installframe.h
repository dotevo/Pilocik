#ifndef INSTALLFRAME_H
#define INSTALLFRAME_H

#include "resourcesmanager.h"
#include <QFrame>
#include <QList>

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
    QList<MapResource> maps;
    static const int WINCEactivesync = 0;
    static const int WINCEmemorycard = 1;
    static const int PCharddrive = 2;
    int installMode;

private slots:
    void init();
    void on_installBtn_clicked();
    void on_mapsList_itemSelectionChanged();
    void on_winCEbtn_clicked();
    void on_PCbtn_clicked();
    void deviceStatus(int status);
};

#endif // INSTALLFRAME_H
