#include "filedownload.h"
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringList>
#include <QTimer>
#include <QUrl>
#include <QDebug>
#include <stdio.h>

FileDownload::FileDownload()
 {
     connect(&manager, SIGNAL(finished(QNetworkReply*)),
             SLOT(downloadFinished(QNetworkReply*)));
 }

 void FileDownload::doDownload(const QUrl &url, QString savePath)
 {
     QNetworkRequest request(url);
     QNetworkReply *reply = manager.get(request);
     this->savePath = savePath;

     connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
             SLOT(downloadProgress(qint64,qint64)));

     currentDownloads.append(reply);
 }

 QString FileDownload::saveFileName(const QUrl &url, bool rewrite)
 {
     QString path = url.path();
     QString basename = QFileInfo(path).fileName();

     if (basename.isEmpty())
         basename = "download";

     if (!rewrite && QFile::exists(basename)) {
         // already exists, don't overwrite
         int i = 0;
         basename += '.';
         while (QFile::exists(basename + QString::number(i)))
             ++i;

         basename += QString::number(i);
     }

     return savePath+basename;
 }

 bool FileDownload::saveToDisk(const QString &filename, QIODevice *data)
 {
     QFile file(filename);
     if (!file.open(QIODevice::WriteOnly)) {
         fprintf(stderr, "Could not open %s for writing: %s\n",
                 qPrintable(filename),
                 qPrintable(file.errorString()));
         return false;
     }

     file.write(data->readAll());
     file.close();

     return true;
 }

 void FileDownload::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
 {
    emit progress(bytesReceived, bytesTotal);
 }


 void FileDownload::downloadFinished(QNetworkReply *reply)
 {
     QUrl url = reply->url();
     if (reply->error()) {
         fprintf(stderr, "Download of %s failed: %s\n",
                 url.toEncoded().constData(),
                 qPrintable(reply->errorString()));
     } else {
         QString filename = saveFileName(url);
         if (saveToDisk(filename, reply))
             printf("Download of %s succeeded (saved to %s)\n",
                    url.toEncoded().constData(), qPrintable(filename));
         emit finished(filename);
     }

     currentDownloads.removeAll(reply);
     reply->deleteLater();
 }
