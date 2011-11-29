#ifndef PILIBOCIK_PARTITIONFILE_H
#define PILIBOCIK_PARTITIONFILE_H

#include <QFile>
#include <QIODevice>
#include <pilibocik/boundarybox.h>
#include <pilibocik/geohash.h>

namespace PiLibocik{namespace Partition{
class Way;
class Node;
class PartitionFile;

class WayFile: public QFile{
private:
    QDataStream     *stream;
    int fileType;
    PartitionFile *part;

public:
    WayFile(QString filename,int fileType,PartitionFile *p=0);
    Way getWay(qint64 pos);

};

class NodeFile: public QFile{
private:
    QDataStream     *stream;
    int fileType;
    PartitionFile *part;
public:
    NodeFile(QString filename,int fileType,PartitionFile *p=0);
    Node getNode(qint64 pos=-1);
    QList<Node> getBlock(qint64 pos);
};

class IndexNodeFile:public QFile{
private:
    PartitionFile *part;
public:
    IndexNodeFile(QString filename,PartitionFile *p=0);
    int getPrecision();
    qint64 getNodesBlock(Geohash geo);
};

class PrioritetsFile:public QFile{
private:
    QDataStream *stream;
    int fileType;
    PartitionFile *part;
public:
    PrioritetsFile(QString filename,int fileType,PartitionFile *p=0);
    double getPrioritet(qint64 pos);
};

class PartitionFile
{
private:
    IndexNodeFile   *indexNodeFile;
    NodeFile        *nodeFile;
    WayFile         *wayFile;
    PrioritetsFile  *prioritetsFile;
    int sizeType;

#ifdef PiLibocik_WRITE_MODE
    void addIndex(QDataStream &stream,qint64 pos,int type);
    void addWayToFile(QDataStream &waystream,QDataStream &wayStream,Way *w,QMap<int,qint64 > &waysIndex,QMap<int,int> &waysIndexId,QMap<int,qint64 > &nodesIndex, QList<QPair <int,qint64> > &waysToReplaceInWays,int sizeType);
#endif

public:

    PartitionFile(QString path, QString priotype,  QFile::OpenMode flag,int fileType);
    ~PartitionFile();
    QList <Node> getNodesFromBoundaryBox(BoundaryBox &bbox);
    IndexNodeFile   *getIndexNodeFile();
    NodeFile        *getNodeFile();
    WayFile         *getWayFile();
    PrioritetsFile  *getPrioritetsFile();
    int             getSizeType();

#ifdef PiLibocik_WRITE_MODE
    void savePartition( QList<Way> &ways, QList<Node> &nodes, int prec);
#endif

    /**
     * @brief Gets node that is closest to given position.
     *
     * @param pos - given position
     *
     * @return Closest node
     */
    Node getNearestNode(Position pos);
    static qint64 loadIndex(QDataStream &stream,int type);
};


}}

#endif // PARTITIONFILE_H
