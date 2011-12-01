#include <QtGui/QApplication>
#include <QDebug>
#include "mainwindow.h"
#include <pilibocik/partition/node.h>
#include <pilibocik/partition/partitionfile.h>
#include <pilibocik/partition/way.h>
#include <pilibocik/partition/edge.h>
#include <pilibocik/boundarybox.h>
#include <pilibocik/poi.h>
#include <pilibocik/poifileppoi.h>
#include <pilibocik/geohash.h>

int main(int argc, char *argv[])
{
    /*
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();*/

// POI SAVE
    QList<PiLibocik::Poi> pois;
    QList < QPair <QString, QString> >  tags;
    QString geo1=PiLibocik::Geohash::generateGeohash(20,10,5);
    PiLibocik::Poi poi(20,10,QString::fromUtf8("ąasdęś"),0,tags,geo1);
    qDebug()<<"POI:"<<poi.getName();
    pois.append(poi);
    QString geo2=PiLibocik::Geohash::generateGeohash(19.1,10.0,5);
    PiLibocik::Poi poi2(10.1,10,QString::fromUtf8("Kąpłęta"),0,tags,geo2);
    pois.append(poi2);
    QMap<int,QString> types;
    types.insert(0,QString::fromUtf8("ędłę"));
    PiLibocik::PoiFilePPOI ppo;
    ppo.saveToFile("poi",pois,types);


        PiLibocik::PoiFilePPOI n;
        PiLibocik::BoundaryBox b(PiLibocik::Position(20,10),PiLibocik::Position(20,10));
        QTime t;
        t.start();
        QList <PiLibocik::Poi> p=n.loadPOIsFromFile("poi",b,-1);
        QListIterator <PiLibocik::Poi> iter(p);
        int i = 0;
        while(iter.hasNext()){
            PiLibocik::Poi poi=iter.next();
            i++;
            qDebug()<<poi.getLat()<<":"<<poi.getLon()<<" Name:"<<poi.getName()<<" Type:"<<poi.getType()<<"Geohash:"<<poi.getGeohash();
            QList < QPair <QString,QString > > l= poi.getTags();
            QListIterator <QPair <QString,QString > > iter2(l);
            while(iter2.hasNext()){
                QPair <QString,QString > n=iter2.next();
                qDebug()<<n.first<<"="<<n.second;
            }
        }
        qDebug()<<"Time: "<<t.elapsed()<<"Size: "<<i;




  /*  {

         //Tworzenie przyk3adowych danych
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
         p.savePartition(ways,nodes,4);
     }
     PiLibocik::Partition::PartitionFile p("test","car",QIODevice::ReadOnly,1);
     PiLibocik::BoundaryBox bbox(PiLibocik::Position(0.1,0.1),PiLibocik::Position(1.5,1.5));
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
*/

    return 0;
}
