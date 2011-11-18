#include <pilibocik/geohash.h>
#include <pilibocik/partition/partitionfile.h>
#include <pilibocik/partition/node.h>

#include <QDebug>

int main(){
    //Tworzenie przyk³adowych danych
    QList <PiLibocik::Partition::Node> nodes;
    PiLibocik::Partition::Way w(1,0.5,(quint8)0);
    //Dodaj 10node do drogi 1
    for(int i=0;i<10;i++){
        PiLibocik::Partition::Node n(i,0.5*i,0.8*i);
        PiLibocik::Partition::Edge e1( (i+1)%10,0.1 );
        //PiLibocik::Partition::Edge e2
        n.addBoundaryEdge(e1);
        n.addWay(1);
        nodes.append(n);
        w.addNode(i);
    }
    PiLibocik::Partition::PartitionFile p("test","car",QIODevice::WriteOnly);
    QList <PiLibocik::Partition::Way> ways;
    ways.append(w);
    p.savePartition(ways,nodes,2,1);



    return 0;
}
