#ifndef MAPOPTIONSWINDOW_H
#define MAPOPTIONSWINDOW_H

#include <QFrame>

namespace Ui {
    class MapOptionsWindow;
}

class MapOptionsWindow : public QFrame
{
    Q_OBJECT

public:
    explicit MapOptionsWindow(QWidget *parent = 0);
    ~MapOptionsWindow();

private:
    Ui::MapOptionsWindow *ui;
};

#endif // MAPOPTIONSWINDOW_H
