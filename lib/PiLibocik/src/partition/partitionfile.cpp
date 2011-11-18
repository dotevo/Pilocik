#include <pilibocik\partition\partitionfile.h>
#include <pilibocik/partition/edge.h>
#include <QMap>
#include <QDebug>
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
    stream=new QDataStream(this);
    prioFile=0;
}

void NodeFile::setPrioritetsFile(PrioritetsFile *prio){
    prioFile=prio;
}

Node NodeFile::getNode(qint64 pos){
    if(pos!=-1)
        stream->device()->seek(pos);

    quint32 id;
    double lon,lat;
    quint8 waysCount,boundCount,routeCount;
    stream->operator >>(id);
    stream->operator >>(lon);
    stream->operator >>(lat);
    qDebug()<<id<<":"<<lon<<":"<<lat;
    Node node(id,lon,lat);
    stream->operator >>(waysCount);
    for(quint8 i=0;i<waysCount;i++){
        quint64 w;
        stream->operator >>(w);
        node.addWay(w);
    }
    if(prioFile==0){
        qDebug()<<"NodeFile: PrioritetFile needs set.";
        return node;
    }

    stream->operator >>(boundCount);
    for(quint8 i=0;i<boundCount;i++){
        qint64 nInd,prioInd;
        stream->operator >>(nInd);
        stream->operator >>(prioInd);
        double value=prioFile->getPrioritet(prioInd);
        Edge e(nInd,value);
        node.addBoundaryEdge(e);
    }
    stream->operator >>(routeCount);
    for(quint8 i=0;i<routeCount;i++){
        qint64 nInd,prioInd;
        stream->operator >>(nInd);
        stream->operator >>(prioInd);
        double value=prioFile->getPrioritet(prioInd);
        Edge e(nInd,value);
        node.addBoundaryEdge(e);
    }
    return node;
}


QList<Node> NodeFile::getBlock(qint64 pos){
    QList<Node> n;
    if(!isOpen())
        return n;

    QDataStream ds(this);
    quint32 nodesCount;
    ds.device()->seek(pos);
    ds>>nodesCount;
    for(quint32 i=0;i<nodesCount;i++){
        //-1 czyli nie zmieniaj wartosci (czytaj kolejno)
        Node node=getNode(-1);
        n.append(node);
    }
    return n;
}


IndexNodeFile::IndexNodeFile(QString filename):QFile(filename){
}

int IndexNodeFile::getPrecision(){
    QDataStream dataStream(this);
    quint8 value=0;
    dataStream>>value;
    qDebug()<<(int)value;
    return (int)value;
}

qint64 IndexNodeFile::getNodesBlock(Geohash geo){
    //TODO
    return 0;
}

PrioritetsFile::PrioritetsFile(QString filename):QFile(filename){
    stream=new QDataStream(this);
}

double PrioritetsFile::getPrioritet(qint64 pos){
    stream->device()->seek(pos);
    double value;
    stream->operator >>(value);
    return value;
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
    prioritetsFile  = new PrioritetsFile(path+"_"+prioType+".prio");
    indexNodeFile   = new IndexNodeFile(path+".idx");    
    nodeFile        = new NodeFile(path+".node");
    nodeFile->setPrioritetsFile(prioritetsFile);
    wayFile         = new WayFile(path+".way");

    //Open files
    indexNodeFile->open(flag);
    nodeFile->open(flag);
    wayFile->open(flag);
    prioritetsFile->open(flag);
}

PartitionFile::~PartitionFile(){
    //Close files
    indexNodeFile->close();
    nodeFile->close();
    wayFile->close();
    prioritetsFile->close();
    delete indexNodeFile;
    delete nodeFile;
    delete wayFile;
    delete prioritetsFile;
}

QList <Node> PartitionFile::getNodesFromBoundaryBox(BoundaryBox &bbox){
    QList <Node> ret;

    if(!indexNodeFile->isOpen())
        return ret;

    QList <Geohash> geohashes=bbox.getGeohashesIn(indexNodeFile->getPrecision());
    QListIterator <Geohash> iter(geohashes);
    while(iter.hasNext()){
        Geohash g=iter.next();
        qint64 index=indexNodeFile->getNodesBlock(g);
        ret.append( nodeFile->getBlock(index)  );
    }

    return ret;
}

#ifdef PiLibocik_WRITE_MODE
void PartitionFile::addIndex(QDataStream &stream,qint64 pos,int type){
    //8byte
    if(type==0){
        stream<<pos;
    }
    //4 byte
    else if(type==1){
        qint32 p=pos;
        stream<<p;
    }
}


void PartitionFile::savePartition( QList<Way> &ways, QList<Node> &nodes, int prec, int sizeType){
    qDebug()<<"Do zapisania:"+QString::number(ways.size())+"dróg,"+QString::number(nodes.size())+"node";
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

    qDebug()<<"Bloków geohashedNodes:"+QString::number(geoHashedNodes.size());

    //SAVE NODES---------------------------------------------
        //Helper values (remeber position in file)
        QMap<Geohash,qint64 > geoBlocks;
        QMap<int,qint64 > nodesIndex;
        QMap<int,qint64 > waysIndex;

        //To change (replace values)
        QList<QPair <int,qint64> > waysToReplace;
        QList<QPair <int,qint64> > nodesToReplace;

        QMapIterator <Geohash ,QVector<Node*> *> mIter(geoHashedNodes);
        if(!mIter.hasNext()){
            qDebug("Bubel!");
            return;
        }

        QList<Geohash> geokeys=geoHashedNodes.keys();
        qSort(geokeys);

        {
            qint64 emptyIndex=0;
            QListIterator<Geohash> listIterator(geokeys);
            Geohash geo(0);
            while(listIterator.hasNext()){
                geo=listIterator.next();
                if(geoHashedNodes.contains(geo)){
                    qDebug()<<"Dodaje";
                    //Dodawanie danych dla indexu
                    geoBlocks.insert(geo,nodeStream.device()->pos());
                    QVector<Node*>* dupa=geoHashedNodes.value(geo);
                    quint32 nodes_block=(quint32)dupa->size();
                    //Dopisz do stream liczbe node w bloku
                    nodeStream << nodes_block;
                    for(int i=0;i<dupa->size();i++){
                        Node *n=dupa->at(i);
                        qDebug()<<"V:"+QString::number(n->getId())+" pos:"+QString::number(nodeStream.device()->pos());
                        //Dodaj node dla indexu
                        nodesIndex.insert(n->getId(),nodeStream.device()->pos());
                        //Dodaj dane w node
                        nodeStream<<n->getId();
                        nodeStream<<n->getLon();
                        nodeStream<<n->getLat();
                        /****WAYS****/
                        //Kiedy zapisywane to w ways sa ID
                        QVector <qint64> node_ways=n->getWays();
                        nodeStream<<(quint8)node_ways.size();
                        for(int j=0;j<node_ways.size();j++){
                            //Dodaj wpis oznaczajacy podmianê
                            waysToReplace.append(QPair<int,qint64>(node_ways.at(j),nodeStream.device()->pos()) );
                            //Dodaj pusty index
                            addIndex(nodeStream,emptyIndex,sizeType);
                        }
                        /****BOUNDARY_EDGES****/
                        QVector <Edge> node_bound=n->getBoundaryEdges();
                        nodeStream<<(quint8)node_bound.size();
                        for(int j=0;j<node_bound.size();j++){
                            //Dodaj wpis oznazcajacy podmiane node
                            Edge e=node_bound.at(j);
                            nodesToReplace.append(QPair<int,qint64>(e.getPair(),nodeStream.device()->pos()) );
                            //Dodaj pusty index
                            addIndex(nodeStream,emptyIndex,sizeType);
                            //Dodaj priorytet index
                            addIndex(nodeStream,prioritetsStream.device()->pos(),sizeType);
                            prioritetsStream<<(double)e.getPrioritet();
                        }
                        /****ROUTING_EDGES****/
                        QVector <Edge> node_route=n->getRoutingEdges();
                        nodeStream<<(quint8)node_route.size();
                        for(int j=0;j<node_route.size();j++){
                            //Dodaj wpis oznazcajacy podmiane node
                            Edge e=node_route.at(j);
                            nodesToReplace.append(QPair<int,qint64>(e.getPair(),nodeStream.device()->pos()) );
                            //Dodaj pusty index
                            addIndex(nodeStream,emptyIndex,sizeType);
                            //Dodaj priorytet index
                            addIndex(nodeStream,prioritetsStream.device()->pos(),sizeType);
                            prioritetsStream<<(double)e.getPrioritet();
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
                addIndex(nodeStream,nodesIndex.value(L.first),sizeType) ;
            }


        }
    //SAVE WAYS----------------------------------------------
        //Dzielenie na 3 grupy (oneway=0;1;-1)
        QVector <Way*> oneway0;
        QVector <Way*> oneway1;
        QVector <Way*> oneway_1;
        QList<QPair <int,qint64> > waysToReplaceInWays;

        QListIterator <Way> waysIterator(ways);
        while(waysIterator.hasNext()){
            Way *w=(Way*)&waysIterator.next();
            if(w->getOneway()==0)
                oneway0.push_back(w);
            else if(w->getOneway()==-1)
                oneway_1.push_back(w);
            else if(w->getOneway()==1)
                oneway1.push_back(w);
        }
        //oneway 1 pos
        addIndex(wayStream,0,sizeType) ;
        //oneway -1 pos
        addIndex(wayStream,0,sizeType) ;
        for(int i=0;i<oneway0.size();i++){
            Way *w=oneway0.at(i);
            addWayToFile(wayStream,prioritetsStream,w,waysIndex,nodesIndex,waysToReplaceInWays,sizeType);
        }
        qint64 wo_1=wayStream.device()->pos();
        for(int i=0;i<oneway1.size();i++){
            Way *w=oneway1.at(i);
            addWayToFile(wayStream,prioritetsStream, w,waysIndex,nodesIndex,waysToReplaceInWays,sizeType);
        }
        qint64 wo_0=wayStream.device()->pos();
        for(int i=0;i<oneway_1.size();i++){
            Way *w=oneway_1.at(i);
            addWayToFile(wayStream,prioritetsStream,w,waysIndex,nodesIndex,waysToReplaceInWays,sizeType);
        }

        //Uzupe³nianie brakujacych w way
        QListIterator<QPair <int,qint64> > uzupelniaczWayInWay(waysToReplaceInWays);
        while(uzupelniaczWayInWay.hasNext()){
            QPair <int,qint64> L=uzupelniaczWayInWay.next();
            //Ustaw na pozycje
            wayStream.device()->seek(L.second);
            addIndex(wayStream,waysIndex.value(L.first),sizeType) ;
        }
        wayStream.device()->seek(0);
        addIndex(wayStream,wo_1,sizeType) ;
        addIndex(wayStream,wo_0,sizeType) ;
    //-------------------------------------------------------
        //Uzupe³nij waysInNodes
        QListIterator<QPair <int,qint64> > uzupelniaczWayInNodes(waysToReplace);
        while(uzupelniaczWayInNodes.hasNext()){
            QPair <int,qint64> L=uzupelniaczWayInNodes.next();
            //Ustaw na pozycje
            nodeStream.device()->seek(L.second);
            addIndex(nodeStream,waysIndex.value(L.first),sizeType) ;
        }

    //---Budowanie indexów------------------------------------
        QListIterator<Geohash> geoIterator(geokeys);
        //Geohash size
        indexNodeStream<<(quint8)prec;

        bool was=false;
        Geohash geo2(0);
        while(geoIterator.hasNext()){
            Geohash geo=geoIterator.next();
            if(!was){
                //geohash size
                Geohash last=geokeys.at(geokeys.size()-1);
                qDebug()<<geo.toQString();
                qDebug()<<last.toQString();
                quint64 l=last-geo;
                qDebug()<<l;
                //Geohash size
                indexNodeStream<<l;

                QString n=geo.toQString();
                indexNodeStream << n.toLatin1();

                //Dodaj pierwszy
                addIndex(indexNodeStream,geoBlocks.value(geo),sizeType);
                geo2=geo;
                geo=geoIterator.next();
                was=true;

            }
            for(;geo2<geo;geo2++){
                addIndex(indexNodeStream,0,sizeType);
            }

            if(geoBlocks.contains(geo)){
                addIndex(indexNodeStream,geoBlocks.value(geo),sizeType);
            }else{
                //empty
                addIndex(indexNodeStream,0,sizeType);
            }
        }


}


void PartitionFile::addWayToFile(QDataStream &wayStream,QDataStream &prioritetsStream,Way *w,QMap<int,qint64 > &waysIndex,QMap<int,qint64 > &nodesIndex, QList<QPair <int,qint64> > &waysToReplaceInWays, int sizeType){
    waysIndex.insert(w->getId(),wayStream.device()->pos());
    wayStream<<(quint32)w->getId();
    //Nodes
    QVector <qint64> l= w->getNodes();
    wayStream<<(quint16)l.size();
    for(int j=0;j<l.size();j++){
        wayStream<<nodesIndex.value(l.at(j));
    }
    //Prioritet
    addIndex(wayStream,prioritetsStream.device()->pos(),sizeType);
    prioritetsStream<<(double)w->getPrioritet();
    //Restriction
    QVector <Restriction> r=w->getRestrictions();
    wayStream<<(quint8)r.size();
    for(int j=0;j<r.size();j++){
        Restriction rest=r.at(j);
        wayStream<<(nodesIndex.value(rest.getVia()));
        waysToReplaceInWays.append(QPair<int,qint64>(rest.getWayTo(),wayStream.device()->pos()) );
        wayStream<<(qint64)0;
        wayStream<<(quint8)(rest.getType());
    }
}

#endif




}}
