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
    QList<QString> downloadUrls;
    int selectedIdx;
    qint64 lastSize;
    qint64 downloaded;
    QString selectedName;
    long totalSize;

private slots:
    void init();
    void on_downloadBtn_clicked();
    void on_mapsList_itemSelectionChanged();
    void nextDownload();
    void progressUpdate(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();
};

#endif // MAPDOWNLOADFRAME_H
