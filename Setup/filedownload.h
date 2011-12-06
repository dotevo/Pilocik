#ifndef FILEDOWNLOAD_H
#define FILEDOWNLOAD_H

#include <QFile>
#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>


class FileDownload: public QObject
{
    Q_OBJECT
    QNetworkAccessManager manager;
    QList<QNetworkReply *> currentDownloads;
    QString savePath;

public:
    FileDownload();
    void doDownload(const QUrl &url, QString savePath = "");
    QString saveFileName(const QUrl &url, bool rewrite=true);
    bool saveToDisk(const QString &filename, QIODevice *data);

public slots:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished(QNetworkReply *reply);

signals:
    void finished(QString filename);
    void progress(qint64 bytesReceived, qint64 bytesTotal);
};

#endif // FILEDOWNLOAD_H
