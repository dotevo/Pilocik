#ifndef MAPDOWNLOADFRAME_H
#define MAPDOWNLOADFRAME_H

#include <QFrame>

namespace Ui {
    class MapDownloadFrame;
}

class MapDownloadFrame : public QFrame
{
    Q_OBJECT

public:
    explicit MapDownloadFrame(QWidget *parent = 0);
    ~MapDownloadFrame();

private:
    Ui::MapDownloadFrame *ui;
};

#endif // MAPDOWNLOADFRAME_H
