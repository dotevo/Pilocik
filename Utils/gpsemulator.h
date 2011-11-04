#ifndef GPSEMULATOR_H
#define GPSEMULATOR_H

#include <QThread>
#include <QDebug>
#include <QSharedMemory>
#include <QBuffer>
#include <QFile>
#include <QString>
#include <QStringList>

class GPSemulator: public QThread
{
    Q_OBJECT

public:
    GPSemulator();
    void init(QString path);
    void pause();
    void stop();
    bool isPaused();

private:
    QSharedMemory   gpsData;
    QStringList     loadedData;
    int sleepTime,
        currentIter;
    bool stopCondition,
         paused;

    void insertIntoSharedMemory(QString data);

protected:
    void run();

signals:
    void dataSend(QString data);
    void progressUpdate(int percent);

public slots:
    void selectSimPoint(int point);
    void setSpeed(double speed);

};

#endif // GPSEMULATOR_H
