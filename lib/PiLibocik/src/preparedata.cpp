#include <pilibocik/preparedata.h>

#include <QDebug>

namespace PiLibocik{

    PrepareData::PrepareData(QString dbPath, QString XMLconfigPath)
    {
        db = QSqlDatabase::addDatabase("QSQLITE");

        db.setDatabaseName(dbPath);

        db.open();

        loadXMLconfig(XMLconfigPath);
        generateData();

        db.close();
    }

    void PrepareData::loadXMLconfig(QString XMLpath)
    {
        QDomDocument* doc = new QDomDocument();
        QFile file(XMLpath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                qDebug()<<"invalid XML path";
                return;
            }
        }
        doc->setContent(&file);
        file.close();

        QDomElement types = doc->firstChildElement("poi").firstChildElement("type");

        while(!types.isNull()){
            qDebug()<<types.attributeNode("id").value();
            poiTypes.insert(types.attributeNode("id").value().toInt(),
                            QPair<QString,QString>(types.attributeNode("key").value(),types.attributeNode("value").value()));
            poiTypeNames.insert(types.attributeNode("id").value().toInt(),types.attributeNode("name").value());

            QDomElement tags = types.firstChildElement("tag");
            QList<QString> subtags;

            while(!tags.isNull())
            {
                subtags.append(tags.attributeNode("key").value());
                tags = tags.nextSiblingElement("tag");
            }

            if(!subtags.isEmpty())
                poiTypeSubtags.insert(types.attributeNode("id").value().toInt(), subtags);

            types = types.nextSiblingElement("type");
        }
    }

    void PrepareData::generateData()
    {
        QMapIterator<int, QPair<QString,QString> > i(poiTypes);
        while (i.hasNext()) {
            i.next();
            QSqlQuery qnodes;
            QPair<QString,QString>* typeTag = (QPair<QString,QString>*)&(i.value());
            qnodes.exec("SELECT id, lon, lat, hash FROM nodes WHERE id IN (SELECT ref FROM node_tags WHERE tag=(SELECT id FROM tags where key='"+typeTag->first+"' and value='"+typeTag->second+"'))");
            while(qnodes.next())
            {
                int id = qnodes.value(0).toInt();
                double lon = qnodes.value(1).toDouble();
                double lat = qnodes.value(2).toDouble();
                QString hash = qnodes.value(3).toString();

                QSqlQuery qtags;
                qtags.exec("SELECT key,value FROM tags WHERE id IN (SELECT tag FROM node_tags WHERE ref="+QString::number(id)+")");
                QMap<QString,QString> queryTags;
                while(qtags.next())
                    queryTags.insert(qtags.value(0).toString(),qtags.value(1).toString());

                QString name = "";
                if(!queryTags["name"].isNull())
                {
                    name = queryTags["name"];
                    queryTags.remove("name");
                }

                QList< QPair<QString,QString> > tags;

                foreach(QString tag, poiTypeSubtags[i.key()])
                    if(queryTags.keys().contains(tag))
                    {
                        tags.append(QPair<QString,QString>(tag,queryTags[tag]));
                        qDebug()<<tags.size();
                    }

                //poiList.append(Poi(lon,lat,name,i.key(),tags, hash));
                QString strtags;
                for(int x=0; x < tags.size(); x++)
                {
                    strtags.append("(").append(tags.at(x).first).append("=").append((tags.at(x)).second).append(")");
                }
                qDebug()<<lon<<lat<<name<<i.key()<<strtags<<hash;
            }

            QSqlQuery qwaynodes;
            qwaynodes.exec("SELECT way, lon, lat FROM way_nodes INNER JOIN nodes ON nodes.id = node WHERE way IN (SELECT ref FROM way_tags WHERE tag=(SELECT id FROM tags where key='"+typeTag->first+"' and value='"+typeTag->second+"'))");

            while(qwaynodes.next())
            {
                int id = qwaynodes.value(0).toInt();
                double lon = qwaynodes.value(1).toDouble();
                double lat = qwaynodes.value(2).toDouble();

                if(!wayNodes.contains(id))
                {
                    QList<Point> pointList;
                    pointList.append(Point(lon,lat));
                    wayNodes.insert(id, pointList);
                }
                else
                {
                    wayNodes[id].append(Point(lon,lat));
                }

            }

            QMapIterator<int, QList<Point> > j(wayNodes);
            while (j.hasNext()) {
                j.next();
                Point shapeCenter = shapeToPoint(j.value());
                double lon = shapeCenter.getLon();
                double lat = shapeCenter.getLat();
                QString hash = Geohash::generateGeohash(lon,lat,8);

                QSqlQuery qtags;
                qtags.exec("SELECT key,value FROM tags WHERE id IN (SELECT tag FROM way_tags WHERE ref="+QString::number(j.key())+")");
                QMap<QString,QString> queryTags;
                while(qtags.next())
                    queryTags.insert(qtags.value(0).toString(),qtags.value(1).toString());

                QString name = "";
                if(!queryTags["name"].isNull())
                {
                    name = queryTags["name"];
                    queryTags.remove("name");
                }

                QList< QPair<QString,QString> > tags;

                foreach(QString tag, poiTypeSubtags[i.key()]){
                    if(queryTags.keys().contains(tag))
                    {
                        tags.append(QPair<QString,QString>(tag,queryTags[tag]));
                    }
                }
                //poiList.append(Poi(lon,lat,name,i.key(),tags, hash));
                QString strtags;
                for(int x=0; x < tags.size(); x++)
                {
                    strtags.append("(").append(tags.at(x).first).append("=").append((tags.at(x)).second).append(")");
                }
                qDebug()<<lon<<lat<<name<<i.key()<<strtags<<hash;

            }
        }
    }

    Point PrepareData::shapeToPoint(QList<Point> shape)
    {
        double sumLon = 0;
        double sumLat = 0;
        foreach(Point p, shape)
        {
            sumLon += p.getLon();
            sumLat += p.getLat();
        }

        return Point(sumLon/shape.size(),sumLat/shape.size());
    }

}

