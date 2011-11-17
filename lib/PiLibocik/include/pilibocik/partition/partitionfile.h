#ifndef PARTITIONFILE_H
#define PARTITIONFILE_H

#include <QFile>
#include <QIODevice>
#include <pilibocik/partition/way.h>
#include <pilibocik/boundarybox.h>
#include <pilibocik/geohash.h>

namespace PiLibocik{namespace Partition{

class WayFile: public QFile{
public:
    WayFile(QString filename);
    Way getWay(quint64 pos);

};

class NodeFile: public QFile{
public:
    NodeFile(QString filename);
    Node getNode(quint64 pos);
};

class IndexNodeFile:public QFile{
public:
    IndexNodeFile(QString filename);
    qint64 getNodesBlock(Geohash geo);
};

class PrioritetsFile:public QFile{
public:
    PrioritetsFile(QString filename);
    double getPrioritet(qint64 pos);
};

class PartitionFile
{
private:
    IndexNodeFile   *indexNodeFile;
    NodeFile        *nodeFile;
    WayFile         *wayFile;
    PrioritetsFile  *prioritetsFile;
public:
    PartitionFile(QString path, QString priotype,  QFile::OpenMode flag);
    QVector <Node> getNodesFromBoundaryBox(BoundaryBox &bbox);
#ifdef PiLibocik_WRITE_MODE
    void savePartition( QMap<int,Way> &ways, QList<Node> &nodes, int prec);
#endif
};


}}

#endif // PARTITIONFILE_H
