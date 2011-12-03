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

public:
    FileDownload();
    void doDownload(const QUrl &url);
    QString saveFileName(const QUrl &url);
    bool saveToDisk(const QString &filename, QIODevice *data);

public slots:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished(QNetworkReply *reply);

signals:
    void finished(QString filename);
};

#endif // FILEDOWNLOAD_H
