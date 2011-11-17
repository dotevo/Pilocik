#include <pilibocik\partition\partitionfile.h>
#include <pilibocik/partition/edge.h>
#include <QMap>
#include <QListIterator>

namespace PiLibocik{namespace Partition{

WayFile::WayFile(QString filename):QFile(filename){
}

Way WayFile::getWay(quint64 pos){
    //TODO
    Way way;
    return way;
}

NodeFile::NodeFile(QString filename):QFile(filename){
}

Node NodeFile::getNode(quint64 pos){
    //TODO
    Node node;
    return node;
}

IndexNodeFile::IndexNodeFile(QString filename):QFile(filename){
}

qint64 IndexNodeFile::getNodesBlock(Geohash geo){
    //TODO
    return 0;
}

PrioritetsFile::PrioritetsFile(QString filename):QFile(filename){
}

double PrioritetsFile::getPrioritet(qint64 pos){
    //TODO
    return 0.2;
}

/*INDEX(GEOHASH):------------------------------
  geohash size (n)      -quint8
  geohash count(p)      -quint64
  firstgeohash          -char(Latin)*n
  index                 -qint64     *p

  NODES:----------------------------------------
  -----BBOX-----
    Nodes count (nn)    -quint32
    -----Node-----
        int             -quint32
        lon             -double
        lat             -double
        WaysCount(wc)   -quint8
        -----Ways *(wc) ----
            Way         -qint64 (WAYS)
        BoundCount(bc)  -quint8
        ----Boundary *(bc)--
            Node(pair)  -qint64 (NODES)
            Prio        -qint64 (PRIO)
        RouteCount(rc)  -quint8
        ----Routes *(rc)----
            Node(pair)  -qint64 (NODES)
            Prio        -qint64 (PRIO)


  WAYS:-----------------------------------------
  oneway_yes            -qint64
  oneway_-1             -qint64
  -----WAY----------
    id                    -quint32
    Nodes count  (wn)     -quint16
    NODE                  -qint64*wn(NODES FILE)
    PRIO_Index            -qint64  (PRIO FILE)
    Restriction count(wr) -quint8
    ------Restriction----
        via                -qint64 (NODES FILE)
        to                 -qint64 (WAYS FILE)
        type               -quint8 ?????????????
  */





PartitionFile::PartitionFile(QString path, QString prioType,QFile::OpenMode flag){
    //Create new qFiles object
    indexNodeFile   = new IndexNodeFile(path+".idx");
    nodeFile        = new NodeFile(path+".node");
    wayFile         = new WayFile(path+".way");
    prioritetsFile  = new PrioritetsFile(path+"_"+prioType+".prio");
    //Open files
    indexNodeFile->open(flag);
    nodeFile->open(flag);
    wayFile->open(flag);
    prioritetsFile->open(flag);
}

QVector <Node> PartitionFile::getNodesFromBoundaryBox(BoundaryBox &bbox){
    //TODO
    QVector <Node> ret;
    return ret;
}

#ifdef PiLibocik_WRITE_MODE
void PartitionFile::savePartition( QMap<int,Way> &ways, QList<Node> &nodes, int prec){
    //DataStream
    QDataStream indexNodeStream (indexNodeFile);
    QDataStream nodeStream      (nodeFile);
    QDataStream wayStream       (wayFile);
    QDataStream prioritetsStream(prioritetsFile);

    //Convert to geohashed!
    QMap <Geohash, QVector<Node*>* > geoHashedNodes;
    QListIterator <Node> iter(nodes);
    while(iter.hasNext()){
        Node *node = (Node*)&iter.next();
        Geohash geoH=node->getGeohash(prec);
        if(geoHashedNodes.contains(geoH)){
            geoHashedNodes.value(geoH)->push_back(node);
        }else{
            QVector <Node*>* cycki=new QVector<Node*>();
            cycki->push_back(node);
            geoHashedNodes.insert(geoH,cycki);
        }
    }

    //SAVE NODES---------------------------------------------
        //Helper values (remeber position in file)
        QMap<Geohash,qint64 > geoBlocks;
        QMap<int,qint64 > nodesIndex;

        //To change (replace values)
        QList<QPair <int,qint64> > waysToReplace;
        QList<QPair <int,qint64> > nodesToReplace;

        QMapIterator <Geohash ,QVector<Node*> *> mIter(geoHashedNodes);
        if(!mIter.hasNext()){
            qDebug("Bubel!");
            return;
        }

        {
            qint64 emptyIndex=0;
            QList<Geohash> geokeys=geoHashedNodes.keys();
            qSort(geokeys);
            QListIterator<Geohash> listIterator(geokeys);
            Geohash geo(0);
            while(listIterator.hasNext()){
                geo=listIterator.next();
                if(geoHashedNodes.contains(geo)){
                    //Dodawanie danych dla indexu
                    geoBlocks.insert(geo,nodeStream.device()->pos());
                    QVector<Node*>* dupa=geoHashedNodes.value(geo);
                    quint32 nodes_block=(quint32)dupa->size();
                    //Dopisz do stream liczbe node w bloku
                    nodeStream << nodes_block;
                    for(int i=0;i<dupa->size();i++){
                        Node *n=dupa->at(i);
                        //Dodaj node dla indexu
                        nodesIndex.insert(n->getId(),nodeStream.device()->pos());
                        //Dodaj dane w node
                        nodeStream<<n->getId();
                        nodeStream<<n->getLon();
                        nodeStream<<n->getLat();
                        /****WAYS****/
                        //Kiedy zapisywane to w ways sa ID
                        QVector <qint64> node_ways=n->getWays();
                        nodeStream<<node_ways.size();
                        for(int j=0;j<node_ways.size();j++){
                            //Dodaj wpis oznaczajacy podmianê
                            waysToReplace.append(QPair<int,qint64>(node_ways.at(j),nodeStream.device()->pos()) );
                            //Dodaj pusty index
                            nodeStream<<emptyIndex;
                        }
                        /****BOUNDARY_EDGES****/
                        QVector <Edge> node_bound=n->getBoundaryEdges();
                        nodeStream<<node_bound.size();
                        for(int j=0;j<node_bound.size();j++){
                            //Dodaj wpis oznazcajacy podmiane node
                            Edge e=node_bound.at(j);
                            nodesToReplace.append(QPair<int,qint64>(e.getPair(),nodeStream.device()->pos()) );
                            //Dodaj pusty index
                            nodeStream<<emptyIndex;
                            //Dodaj priorytet index
                            nodeStream<<prioritetsStream.device()->pos();
                            prioritetsStream<<e.getPrioritet();
                        }
                        /****ROUTING_EDGES****/
                        QVector <Edge> node_route=n->getRoutingEdges();
                        nodeStream<<node_route.size();
                        for(int j=0;j<node_route.size();j++){
                            //Dodaj wpis oznazcajacy podmiane node
                            Edge e=node_route.at(j);
                            nodesToReplace.append(QPair<int,qint64>(e.getPair(),nodeStream.device()->pos()) );
                            //Dodaj pusty index
                            nodeStream<<emptyIndex;
                            //Dodaj priorytet index
                            nodeStream<<prioritetsStream.device()->pos();
                            prioritetsStream<<e.getPrioritet();
                        }
                    }
                }
            }
            //Uzupe³nianie brakujacych nodeIndex w node
            QListIterator<QPair <int,qint64> > uzupelniacz(nodesToReplace);
            while(uzupelniacz.hasNext()){
                QPair <int,qint64> L=uzupelniacz.next();
                //Ustaw na pozycje
                nodeStream.device()->seek(L.second);
                nodeStream<< nodesIndex.value(L.first);
            }
        }
    //-------------------------------------------------------

}
#endif




}}
