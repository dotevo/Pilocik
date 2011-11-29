#include <QtGui/QApplication>
#include <QDebug>
#include "mainwindow.h"
#include <pilibocik/partition/node.h>
#include <pilibocik/partition/partitionfile.h>
#include <pilibocik/partition/way.h>
#include <pilibocik/partition/edge.h>
#include <pilibocik/boundarybox.h>

int main(int argc, char *argv[])
{
    /*
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();*/

    {
         //Tworzenie przyk3adowych danych
         QList <PiLibocik::Partition::Node> nodes;
         PiLibocik::Partition::Way w(1,0.5,(quint8)0);
         PiLibocik::Partition::Way w2(2,0.5,(quint8)0);
         //Dodaj 10node do drogi 1
         for(int i=0;i<10;i++){
             PiLibocik::Partition::Node n(i, 100,0.1*i+0.1,0.1*i+0.1);
             PiLibocik::Partition::Edge e1( (i+1)%10,  0.1 );
             PiLibocik::Partition::BoundaryEdge b1((i+4)%10,1,0.5);
             //PiLibocik::Partition::Edge e2
             n.addRoutingEdge(e1);
             n.addBoundaryEdge(b1);
             n.addWay(0);

             w.addNode(i);
             if(i%2==0){
                 n.addWay(1);
                 w2.addNode(i);
             }
             nodes.append(n);

         }
         PiLibocik::Partition::PartitionFile p("test","car",QIODevice::WriteOnly,1);
         QList <PiLibocik::Partition::Way> ways;
         ways.append(w);
         ways.append(w2);
         p.savePartition(ways,nodes,4);
     }

     PiLibocik::Partition::PartitionFile p("","car",QIODevice::ReadOnly,1);
     //Wroc³aw 51.118552&lon=17.057824
     PiLibocik::BoundaryBox bbox(PiLibocik::Position(17.0,51),PiLibocik::Position(18,52));
     QList<PiLibocik::Partition::Node> n=p.getNodesFromBoundaryBox(bbox);
     qDebug()<<n.length()<<"LL";
     QListIterator<PiLibocik::Partition::Node> iter(n);
     while(iter.hasNext()){         
         PiLibocik::Partition::Node n=iter.next();         
         QVector<PiLibocik::Partition::Way> ways=n.getWaysObj();
         qDebug()<<"NODE ID"<<n.getId()<<" ways:"<<ways.size();
         for(int i=0;i<ways.size();i++){
             PiLibocik::Partition::Way w=ways.at(i);
             qDebug()<<"   Droga "<<w.getId()<<"z"<<ways.size()<<"w"<<n.getId();
             QVector<PiLibocik::Partition::Node> nodes=w.getNodesObj();
             for(int j=0;j<nodes.size();j++){
                 PiLibocik::Partition::Node nnn=nodes.at(j);
                 qDebug()<<"      NODE"<<nnn.getId()<<":"<<nnn.getCell()<<";"<<nnn.getLon()<<":"<<nnn.getLat();
             }
         }
     }


    return 0;
}
