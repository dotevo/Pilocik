#include <pilibocik/geohash.h>
#include <pilibocik/partition/partitionfile.h>
#include <pilibocik/partition/node.h>
#include <pilibocik/partition/way.h>

#include <QDebug>

int main(){

        {
    //Tworzenie przyk³adowych danych
    QList <PiLibocik::Partition::Node> nodes;
    PiLibocik::Partition::Way w(1,0.5,(quint8)0);
    //Dodaj 10node do drogi 1
    for(int i=0;i<10;i++){
        PiLibocik::Partition::Node n(i,100,0.1*i+0.1,0.1*i+0.1);
        PiLibocik::Partition::Edge e1( (i+1)%10,0.1 );
        //PiLibocik::Partition::Edge e2
        n.addRoutingEdge(e1);
        n.addWay(1);
        nodes.append(n);
        w.addNode(i);
    }
    PiLibocik::Partition::PartitionFile p("test","car",QIODevice::WriteOnly,1);
    QList <PiLibocik::Partition::Way> ways;
    ways.append(w);
    p.savePartition(ways,nodes,2);
    }

    PiLibocik::Partition::PartitionFile p("test","car",QIODevice::ReadOnly,1);
    PiLibocik::BoundaryBox bbox(PiLibocik::Position(0.1,0.1),PiLibocik::Position(0.6,0.5));
    QList<PiLibocik::Partition::Node> n=p.getNodesFromBoundaryBox(bbox);
    QListIterator<PiLibocik::Partition::Node> iter(n);
    while(iter.hasNext()){
        PiLibocik::Partition::Node n=iter.next();        
        QVector<PiLibocik::Partition::Way> ways=n.getWaysObj();
        for(int i=0;i<ways.size();i++){
            PiLibocik::Partition::Way w=ways.at(i);
            qDebug()<<"Droga "<<w.getId();
            QVector<PiLibocik::Partition::Node> nodes=w.getNodesObj();
            for(int j=0;j<nodes.size();j++){
                PiLibocik::Partition::Node nnn=nodes.at(j);
                qDebug()<<" NODE"<<nnn.getId()<<":"<<nnn.getCell()<<";"<<nnn.getLon()<<":"<<nnn.getLat();
            }
        }
    }




    return 0;
}
