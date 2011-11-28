#include <pilibocik/partition/partitionfile.h>
#include <QMap>
#include <QDebug>
#include <QListIterator>

#include <pilibocik/partition/boundaryedge.h>
#include <pilibocik/partition/way.h>
#include <pilibocik/partition/restriction.h>
#include <pilibocik/partition/node.h>
#include <pilibocik/partition/edge.h>


namespace PiLibocik{namespace Partition{

WayFile::WayFile(QString filename,int fileType,PartitionFile *p):QFile(filename),fileType(fileType),part(p){
    stream=new QDataStream(this);
}

Way WayFile::getWay(qint64 pos){
    stream->device()->seek(pos);
    //get ID
    quint32 id;
    stream->operator >>(id);
    //get Nodes
    quint16 nodesCount;
    stream->operator >>(nodesCount);
    QVector <qint64> nodes;
    for(quint16 i=0;i<nodesCount;i++){
        nodes.push_back(PartitionFile::loadIndex(*stream,fileType));
    }

    //get Prio
    qint64 prioInd=PartitionFile::loadIndex(*stream,fileType);
    double prio=part->getPrioritetsFile()->getPrioritet(prioInd);

    //get Restriction
    quint8 restrictionCount;
    QVector <Restriction> restrictions;
    stream->operator >>(restrictionCount);
    for(quint8 i=0;i<restrictionCount;i++){
        qint64 to=PartitionFile::loadIndex(*stream,fileType);
        qint64 via=PartitionFile::loadIndex(*stream,fileType);
        quint8 type;
        stream->operator >>(type);
        Restriction r(to,via,type);
        restrictions.push_back(r);
    }
    stream->device()->seek(0);
    qint64 oneway1,oneway_1;
    stream->operator >>(oneway1);
    stream->operator >>(oneway_1);

    qint8 oneway;
    if(pos>=oneway_1){
        oneway=-1;
    }else if(pos>=oneway1)
        oneway=1;
    else
        oneway=0;

    Way way(id,prio,oneway,part);
    for(int i=0;i<nodes.size();i++){
        way.addNode(nodes.at(i));
    }
    for(int i=0;i<restrictions.size();i++){
        way.addRestriction(restrictions.at(i));
    }
    return way;
}

NodeFile::NodeFile(QString filename,int fileType,PartitionFile *p):QFile(filename),fileType(fileType),part(p){
    stream=new QDataStream(this);
}

Node NodeFile::getNode(qint64 pos){
    if(pos!=-1)
        stream->device()->seek(pos);
    quint32 id;
    quint32 cell;
    double lon,lat;
    quint8 waysCount,boundCount,routeCount;
    stream->operator >>(id);
    stream->operator >>(cell);
    stream->operator >>(lon);
    stream->operator >>(lat);
    Node node(id,cell,lon,lat,part);
    //qDebug()<<id<<":"<<lon<<":"<<lat<<":"<<cell;

    stream->operator >>(waysCount);
    for(quint8 i=0;i<waysCount;i++){
        qint64 w= PartitionFile::loadIndex(*stream,fileType);
        node.addWay(w);
    }

    if(part->getPrioritetsFile()==0){
        qDebug()<<"NodeFile: PrioritetFile needs set.";
        return node;
    }

    stream->operator >>(boundCount);
    //qDebug()<<"Bound"<<boundCount;
    for(quint8 i=0;i<boundCount;i++){
        qint64 nInd     = PartitionFile::loadIndex(*stream,fileType);
        qint64 wayInd  = PartitionFile::loadIndex(*stream,fileType);
        qint64 prioInd  = PartitionFile::loadIndex(*stream,fileType);
        double value=part->getPrioritetsFile()->getPrioritet(prioInd);
        BoundaryEdge e(nInd,wayInd,value,part);
        node.addBoundaryEdge(e);
    }

    stream->operator >>(routeCount);
    //qDebug()<<routeCount;
    for(quint8 i=0;i<routeCount;i++){
        qint64 nInd     = PartitionFile::loadIndex(*stream,fileType);
        qint64 prioInd  = PartitionFile::loadIndex(*stream,fileType);
        double value=part->getPrioritetsFile()->getPrioritet(prioInd);
        Edge e(nInd,value,part);
        node.addRoutingEdge(e);
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
    qDebug()<<"Nodes Count"<<nodesCount;
    for(quint32 i=0;i<nodesCount;i++){
        //-1 czyli nie zmieniaj wartosci (czytaj kolejno)
        Node node=getNode();
        n.append(node);
    }
    return n;
}


IndexNodeFile::IndexNodeFile(QString filename,PartitionFile *p):QFile(filename),part(p){
}

int IndexNodeFile::getPrecision(){
    QDataStream dataStream(this);
    quint8 value=0;
    dataStream>>value;
    qDebug()<<(int)value;
    return (int)value;
}

qint64 IndexNodeFile::getNodesBlock(Geohash geo){
    QDataStream dataStream(this);
    dataStream.device()->seek(0);
    quint8 geoHashSize=0;
    quint64 geoHashsCount=0;
    dataStream >> geoHashSize;
    dataStream >> geoHashsCount;

    char *firstGeo=new char[geoHashSize];
    dataStream.readRawData(firstGeo,geoHashSize);

    QString s(firstGeo);
    s=s.left(geoHashSize);
    Geohash geoFirst(s);
    qint64 n=geo-geoFirst;

    if(n>geoHashsCount||n<0)
        return -1;

    if(part->getSizeType()==1){
        dataStream.device()->seek(dataStream.device()->pos()+n*sizeof(qint32));
        qint32 zz;
        dataStream>>zz;
        if(zz==0)return -1;
        return zz;
    }else if(part->getSizeType()==0){
        dataStream.device()->seek(dataStream.device()->pos()+n*sizeof(qint64));
        qint64 zz;
        dataStream>>zz;
        if(zz==0)return -1;
        return zz;
    }

    return -1;
}

PrioritetsFile::PrioritetsFile(QString filename,int fileType,PartitionFile *p):QFile(filename),fileType(fileType),part(p){
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
        cell            -quint32
        lon             -double
        lat             -double
        WaysCount(wc)   -quint8
        -----Ways *(wc) ----
            Way         -qint64 (WAYS)
        BoundCount(bc)  -quint8
        ----Boundary *(bc)--
            Node(pair)  -qint64 (NODES)
            Way         -qint64 (WAY)
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





PartitionFile::PartitionFile(QString path, QString prioType,QFile::OpenMode flag,int fileType):sizeType(fileType){
    //Create new qFiles object
    prioritetsFile  = new PrioritetsFile(path+"_"+prioType+".prio",fileType,this);
    indexNodeFile   = new IndexNodeFile(path+".idx",this);
    nodeFile        = new NodeFile(path+".node",fileType,this);
    wayFile         = new WayFile(path+".way",fileType,this);

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
        qDebug()<<g.toQString()<<":"<<index<<"Idx";
        if(index>-1)
            ret.append( nodeFile->getBlock(index)  );
    }

    return ret;
}

IndexNodeFile   *PartitionFile::getIndexNodeFile(){
    return indexNodeFile;
}

NodeFile        *PartitionFile::getNodeFile(){
    return nodeFile;
}
WayFile         *PartitionFile::getWayFile(){
    return wayFile;
}

PrioritetsFile  *PartitionFile::getPrioritetsFile(){
    return prioritetsFile;
}

int PartitionFile::getSizeType(){
    return sizeType;
}

Node PartitionFile::getNearestNode(Position pos){
    // TODO: implementation
}

qint64 PartitionFile::loadIndex(QDataStream &stream,int type){
    qint64 ret=0;
    //8byte
    if(type==0){
        qint64 p;
        stream>>p;
        ret=p;
    }
    //4 byte
    else if(type==1){
        qint32 p;
        stream>>p;
        ret=p;
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


void PartitionFile::savePartition( QList<Way> &ways, QList<Node> &nodes, int prec){
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
            nodeStream << (quint64)0x00ff00ff00ff00ff;
            qint64 emptyIndex=0;
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
                        nodeStream<<n->getCell();
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
                        QVector <BoundaryEdge> node_bound=n->getBoundaryEdges();
                        nodeStream<<(quint8)node_bound.size();
                        for(int j=0;j<node_bound.size();j++){
                            //Dodaj wpis oznazcajacy podmiane node
                            BoundaryEdge e=node_bound.at(j);
                            nodesToReplace.append(QPair<int,qint64>(e.getPair(),nodeStream.device()->pos()) );
                            //Dodaj pusty index
                            addIndex(nodeStream,emptyIndex,sizeType);
                            ///Droga
                            waysToReplace.append(QPair<int,qint64>(e.getWay(),nodeStream.device()->pos()) );
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
                quint64 l=last-geo+1;
                qDebug()<<l;
                //Geohash size
                indexNodeStream<<l;

                QString n=geo.toQString();
                indexNodeStream.writeRawData(n.toLatin1(),n.length());

                //Dodaj pierwszy
                addIndex(indexNodeStream,geoBlocks.value(geo),sizeType);
                geo2=geo;
                if(!geoIterator.hasNext()){
                    break;
                }
                was=true;

            }
            else{
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


}


void PartitionFile::addWayToFile(QDataStream &wayStream,QDataStream &prioritetsStream,Way *w,QMap<int,qint64 > &waysIndex,QMap<int,qint64 > &nodesIndex, QList<QPair <int,qint64> > &waysToReplaceInWays, int sizeType){
    waysIndex.insert(w->getId(),wayStream.device()->pos());
    wayStream<<(quint32)w->getId();
    //Nodes
    QVector <qint64> l= w->getNodes();
    wayStream<<(quint16)l.size();
    for(int j=0;j<l.size();j++){
        addIndex(wayStream,nodesIndex.value(l.at(j)),sizeType);
    }
    //Prioritet
    addIndex(wayStream,prioritetsStream.device()->pos(),sizeType);
    prioritetsStream<<(double)w->getPrioritet();
    //Restriction
    QVector <Restriction> r=w->getRestrictions();
    wayStream<<(quint8)r.size();
    for(int j=0;j<r.size();j++){
        Restriction rest=r.at(j);
        addIndex(wayStream,nodesIndex.value(rest.getVia()),sizeType);
        waysToReplaceInWays.append(QPair<int,qint64>(rest.getWayTo(),wayStream.device()->pos()) );
        addIndex(wayStream,0,sizeType);
        wayStream<<(quint8)(rest.getType());
    }
}

#endif




}}
