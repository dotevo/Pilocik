#ifndef DATABASEGEN_H
#define DATABASEGEN_H

#include <QThread>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QFile>
#include <QtXml>
#include <QStringList>
#include <QString>

class DatabaseGen: public QThread
{
    Q_OBJECT

    public:
        const static int MAX_TRANSACTION_QUERIES = 100000;

        DatabaseGen();

        void init(QString osmPath, QString outputDir, bool addIndexes);
        bool openDB();
        bool deleteDB();
        bool createTables();
        bool processXML(QString path);
        void createIndexes();

        QString generateGeohash(double lon, double lat);

    private:
        QSqlDatabase db;
        QString osmPath, outputPath;
        bool createIdx;

        QStringList queriesBuffer;
        QMap<QString, int> tagsMap;
        QMap<int, QPair<double,double> > nodesMap;

        void setVersion(QXmlStreamReader& xml);
        void addNode(QXmlStreamReader& xml);
        void addWay(QXmlStreamReader& xml);
        void addRelation(QXmlStreamReader& xml);
        void addMember(QXmlStreamReader& xml, QString relId);
        int  addTag(QString tagKey, QString tagValue);
        void addRelationTag(QString relId, QString tagKey, QString tagValue);
        void addNodeTag(QString nodeId, QString tagKey, QString tagValue);
        void addWayTag(QString wayId, QString tagKey, QString tagValue);
        bool dbInsert(bool forceInsert = false);

    protected:
        void run();

    signals:
        void progressUpdate(int p);
        void statusUpdate(QString status);

};
#endif // DATABASEGEN_H
