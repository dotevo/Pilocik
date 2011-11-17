#include <pilibocik/geohash.h>
#include <pilibocik/partition/partitionfile.h>
#include <pilibocik/partition/node.h>

#include <QDebug>

int main(){
    //Tworzenie przyk³adowych danych
    QList <PiLibocik::Partition::Node> nodes;
    PiLibocik::Partition::Way w(1,(quint8)0);
    //Dodaj 10node do drogi 1
    for(int i=0;i<10;i++){
        PiLibocik::Partition::Node n(i,10,10*i);
        //PiLibocik::Partition::Edge e1
        //PiLibocik::Partition::Edge e2
        //n.addBoundaryEdge();
        n.addWay(1);
        nodes.append(n);
        w.addNode(i);
    }
    PiLibocik::Partition::PartitionFile p("test","car",QIODevice::WriteOnly);
    QMap <int,PiLibocik::Partition::Way> ways;
    ways.insert(w.getId(),w);
    p.savePartition(ways,nodes,3);



    return 0;
}
