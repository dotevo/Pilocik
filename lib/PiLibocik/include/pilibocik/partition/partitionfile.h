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

/**
 * @brief Class used to parse ways file.
 *
 * @class WayFile partitionfile.h "include/pilibocik/partition/partitionfile.h"
 */
class WayFile: public QFile{
private:
    QDataStream     *stream; /**< TODO */
    int fileType; /**< TODO */
    PartitionFile *part; /**< TODO */

public:
/**
 * @brief Constructor
 *
 * @fn WayFile
 * @param filename Path to file
 * @param fileType 0=indexes in files're qint64 1=indexes in files're qint32
 * @param p Main object of file parsers.
 */
    WayFile(QString filename,int fileType,PartitionFile *p=0);
    /**
     * @brief Return way in seek setted on pos.
     *
     * @fn getWay
     * @param pos Position in file.
     */
    Way getWay(qint64 pos);

};

/**
 * @brief Class used to parse nodes file.
 *
 * @class NodeFile partitionfile.h "include/pilibocik/partition/partitionfile.h"
 */
class NodeFile: public QFile{
private:
    QDataStream     *stream; /** Stream */
    int fileType; /**< TODO */
    PartitionFile *part; /**< TODO */
public:
/**
 * @brief
 *
 * @fn NodeFile
 * @param filename
 * @param fileType 0=indexes in files're qint64 1=indexes in files're qint32
 * @param p
 */
    NodeFile(QString filename,int fileType,PartitionFile *p=0);
    /**
     * @brief
     *
     * @fn getNode
     * @param pos
     */
    Node getNode(qint64 pos=-1);
    /**
     * @brief
     *
     * @fn getBlock
     * @param pos
     */
    QList<Node> getBlock(qint64 pos);
};

/**
 * @brief
 *
 * @class IndexNodeFile partitionfile.h "include/pilibocik/partition/partitionfile.h"
 */
class IndexNodeFile:public QFile{
private:
    PartitionFile *part; /**< TODO */
public:
/**
 * @brief
 *
 * @fn IndexNodeFile
 * @param filename
 * @param p
 */
    IndexNodeFile(QString filename,PartitionFile *p=0);
    /**
     * @brief
     *
     * @fn getPrecision
     */
    int getPrecision();
    /**
     * @brief
     *
     * @fn getNodesBlock
     * @param geo
     */
    qint64 getNodesBlock(Geohash geo);
};

/**
 * @brief
 *
 * @class PrioritetsFile partitionfile.h "include/pilibocik/partition/partitionfile.h"
 */
class PrioritetsFile:public QFile{
private:
    QDataStream *stream; /**< TODO */
    int fileType; /**< TODO */
    PartitionFile *part; /**< TODO */
public:
/**
 * @brief
 *
 * @fn PrioritetsFile
 * @param filename
 * @param fileType 0=indexes in files're qint64 1=indexes in files're qint32
 * @param p
 */
    PrioritetsFile(QString filename,int fileType,PartitionFile *p=0);
    /**
     * @brief
     *
     * @fn getPrioritet
     * @param pos
     */
    double getPrioritet(qint64 pos);
};

/**
 * @brief Main partition file class. Use it to save and load files.
 *
 * @class PartitionFile partitionfile.h "include/pilibocik/partition/partitionfile.h"
 */
class PartitionFile
{
private:
    IndexNodeFile   *indexNodeFile; /**< TODO */
    NodeFile        *nodeFile; /**< TODO */
    WayFile         *wayFile; /**< TODO */
    PrioritetsFile  *prioritetsFile; /**< TODO */
    int sizeType; /**< TODO */

#ifdef PiLibocik_WRITE_MODE
    /**
     * @brief
     * To use this function definiction PiLibocik_WRITE_MODE needed
     *
     * @fn addIndex
     */
    void addIndex(QDataStream &stream,qint64 pos,int type);
    /**
     * @brief
     * To use this function definiction PiLibocik_WRITE_MODE needed
     *
     * @fn addWayToFile
     * @param waystream Data way stream
     */
    void addWayToFile(QDataStream &waystream,QDataStream &wayStream,Way *w,QMap<int,qint64 > &waysIndex,QMap<int,int> &waysIndexId,QMap<int,qint64 > &nodesIndex, QList<QPair <int,qint64> > &waysToReplaceInWays,int sizeType);
#endif

public:

/**
 * @brief Constructor
 *
 * @fn PartitionFile
 * @param path Path to files
 * @param priotype Priority file postfix. Like map_postfix.prio.
 * @param flag Open mode
 * @param fileType 0=indexes in files're qint64 1=indexes in files're qint32
 */
    PartitionFile(QString path, QString priotype,  QFile::OpenMode flag,int fileType);
    /**
     * @brief
     *
     * @fn ~PartitionFile
     */
    ~PartitionFile();
    /**
     * @brief Return objects in boundary box
     *
     * @fn getNodesFromBoundaryBox
     * @param bbox Box
     */
    QList <Node> getNodesFromBoundaryBox(BoundaryBox &bbox);
    /**
     * @brief
     *
     * @fn getIndexNodeFile
     */
    IndexNodeFile   *getIndexNodeFile();
    /**
     * @brief
     *
     * @fn getNodeFile
     */
    NodeFile        *getNodeFile();
    /**
     * @brief
     *
     * @fn getWayFile
     */
    WayFile         *getWayFile();
    /**
     * @brief
     *
     * @fn getPrioritetsFile
     */
    PrioritetsFile  *getPrioritetsFile();
    /**
     * @brief File type.
     *
     * @fn getSizeType
     */
    int             getSizeType();

#ifdef PiLibocik_WRITE_MODE
    /**
     * @brief Save data to file. Indexes of object (ways,nodes) are position at list.
     * To use this function definiction PiLibocik_WRITE_MODE needed
     *
     * @fn savePartition
     * @param ways List of ways
     * @param nodes List of nodes
     * @param prec Geohash precision
     */
    void savePartition( QList<Way> &ways, QList<Node> &nodes, int prec);
#endif

    /**
     * @brief Gets node that is closest to given position.
     *
     * @fn getNearestNode
     * @param pos Position
     * @return Closest node
     */
    Node getNearestNode(Position pos);
    /**
     * @brief
     *
     * @fn loadIndex
     * @param stream
     * @param type index type
     */
    static qint64 loadIndex(QDataStream &stream,int type);
};


}}

#endif // PARTITIONFILE_H
