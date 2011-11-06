#include "gpsemulator.h"

GPSemulator::GPSemulator() : gpsData("GPSDATA")
{

}

void GPSemulator::run()
{
    paused = false;
    stopCondition = false;

    while(currentIter<loadedData.size()&&!stopCondition)
    {
        insertIntoSharedMemory(loadedData.at(currentIter));
        emit dataSend(loadedData.at(currentIter));
        emit progressUpdate(currentIter*100/loadedData.size());
        msleep(sleepTime);
        currentIter++;
    }
}

void GPSemulator::insertIntoSharedMemory(QString data)
{
    if (gpsData.isAttached())
    {
        gpsData.detach();
    }

    QBuffer buffer;
    buffer.open( QBuffer::ReadWrite );
    QDataStream out( &buffer );
    out << data;
    int size = buffer.size();

    if ( !gpsData.create( size ) )
    {
        qDebug()<<"Unable to create shared memory block";
        return;
    }

    // Write into the shared memory
    gpsData.lock();
    char *to = (char*)gpsData.data();
    const char *from = buffer.data().data();
    memcpy( to, from, qMin( gpsData.size(), size ) );
    gpsData.unlock();
}

void GPSemulator::init(QString path)
{
    QFile file(path);
    QString output;

    sleepTime = 1000;
    currentIter = 0;
    stopCondition = false;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //emit simStatusUpdate("Unable to open specified file.");
        return;
    }

    //emit simStatusUpdate("Simulation started.");

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        output.append(line);
        if(line.contains("$GPRMC"))
        {
            loadedData.append(output);
            output.clear();
        }
    }

    file.close();
}

void GPSemulator::pause()
{
    stopCondition = true;
    paused = true;
}

bool GPSemulator::isPaused()
{
    return paused;
}

void GPSemulator::stop()
{
    stopCondition = true;
    currentIter = 0;
    emit progressUpdate(0);
}

void GPSemulator::selectSimPoint(int point)
{
    currentIter = point*loadedData.size()/100;
}

void GPSemulator::setSpeed(double speed)
{
    sleepTime = 1/speed * 1000;
}
