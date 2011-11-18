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
    Node getNode(qint64& pos);
    QList<Node> getBlock(qint64 pos);
};

class IndexNodeFile:public QFile{
public:
    IndexNodeFile(QString filename);
    int getPrecision();
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
#ifdef PiLibocik_WRITE_MODE
    void addIndex(QDataStream &stream,qint64 pos,int type);
    void addWayToFile(QDataStream &waystream,QDataStream &wayStream,Way *w,QMap<int,qint64 > &waysIndex,QMap<int,qint64 > &nodesIndex, QList<QPair <int,qint64> > &waysToReplaceInWays,int sizeType);
#endif

public:
    PartitionFile(QString path, QString priotype,  QFile::OpenMode flag);
    ~PartitionFile();
    QList <Node> getNodesFromBoundaryBox(BoundaryBox &bbox);
#ifdef PiLibocik_WRITE_MODE
    void savePartition( QList<Way> &ways, QList<Node> &nodes, int prec,int sizeType);
#endif
};


}}

#endif // PARTITIONFILE_H
