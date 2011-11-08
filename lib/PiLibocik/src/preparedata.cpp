#include <pilibocik/preparedata.h>

#include <QDebug>

namespace PiLibocik{

    PrepareData::PrepareData(QString dbMapPath, QString dbOutPath, QString XMLconfigPath)
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbMapPath);
        db.open();

        loadXMLconfig(XMLconfigPath);
        generateData();

        db.close();

        db.setDatabaseName(dbOutPath);
        db.open();

        createTables();
        saveToDatabase();

        db.close();
    }

    PrepareData::PrepareData(QString dbLoadPath)
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(dbLoadPath);
        db.open();

        loadFromDatabase();

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
            QPair<QString,QString> typeTag = (QPair<QString,QString>)(i.value());
            qnodes.exec("SELECT id, lon, lat, hash FROM nodes WHERE id IN (SELECT ref FROM node_tags WHERE tag=(SELECT id FROM tags where key='"+typeTag.first+"' and value='"+typeTag.second+"'))");
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

                poiList.append(Poi(lon,lat,name,i.key(),tags,hash));
                QString strtags;
                for(int x=0; x < tags.size(); x++)
                {
                    strtags.append("(").append(tags.at(x).first).append("=").append((tags.at(x)).second).append(")");
                }
                qDebug()<<lon<<lat<<name<<i.key()<<strtags<<hash;
            }

            QSqlQuery qwaynodes;
            qwaynodes.exec("SELECT way, lon, lat FROM way_nodes INNER JOIN nodes ON nodes.id = node WHERE way IN (SELECT ref FROM way_tags WHERE tag=(SELECT id FROM tags where key='"+typeTag.first+"' AND value='"+typeTag.second+"'))");

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
                poiList.append(Poi(lon,lat,name,i.key(),tags, hash));
                QString strtags;
                for(int x=0; x < tags.size(); x++)
                {
                    strtags.append("(").append(tags.at(x).first).append("=").append((tags.at(x)).second).append(")");
                }
                qDebug()<<lon<<lat<<name<<i.key()<<strtags<<hash;
                wayNodes.clear();
            }
        }
    }

    void PrepareData::createTables()
    {
        QString createQuery =
                "CREATE TABLE [pois] ("
                "[id] INTEGER  PRIMARY KEY NOT NULL,"
                "[geohash] TEXT  NOT NULL,"
                "[type] INTEGER  NOT NULL,"
                "[lon] REAL  NOT NULL,"
                "[lat] REAL  NOT NULL,"
                "[name] TEXT  NULL"
                ");"
                "CREATE TABLE [poiTypes] ("
                "[type] INTEGER  NOT NULL PRIMARY KEY,"
                "[name] TEXT  NOT NULL"
                ");"
                "CREATE TABLE [poiTags] ("
                "[id] INTEGER  NOT NULL PRIMARY KEY,"
                "[key] TEXT  NOT NULL,"
                "[value] TEXT  NOT NULL"
                ");"
                "CREATE TABLE [poiTags_rel] ("
                "[poi] INTEGER  NOT NULL,"
                "[tag] INTEGER  NOT NULL"
                ")";

        foreach(QString q, createQuery.split(';')){
            QSqlQuery sql;
            sql.exec(q);
        }
    }

    void PrepareData::saveToDatabase()
    {
        QMapIterator<int,QString> i(poiTypeNames);
        while(i.hasNext())
        {
            i.next();
            queriesBuffer.append("INSERT INTO poiTypes VALUES("+QString::number(i.key())+",'"+i.value()+"');");
        }

        int poiIndex=0;
        int tagIndex=0;

        foreach(Poi p, poiList)
        {
            queriesBuffer.append("INSERT INTO pois VALUES("+QString::number(poiIndex)+",'"+
                                 p.getGeohash()+"',"+QString::number(p.getType())+","+
                                 QString::number(p.getLon())+","+
                                 QString::number(p.getLat())+",'"+p.getName()+"');");

            typedef QPair<QString,QString> tagPair;
            foreach(tagPair t, p.getTags())
            {
                queriesBuffer.append("INSERT INTO poiTags VALUES("+QString::number(tagIndex)+",'"+t.first+"','"+t.second+"');");
                queriesBuffer.append("INSERT INTO poiTags_rel VALUES("+QString::number(poiIndex)+","+QString::number(tagIndex)+");");
                tagIndex++;
            }
            poiIndex++;
        }

        QSqlQuery q;
        q.exec("BEGIN TRANSACTION");
        foreach(QString query, queriesBuffer)
        {
            q.exec(query);
        }
        q.exec("END TRANSACTION");
    }

    void PrepareData::loadFromDatabase()
    {
        QSqlQuery q;
        q.exec("SELECT id,geohash,type,lon,lat,name FROM pois;");
        while(q.next())
        {
            QSqlQuery t;
            t.exec("SELECT id,key,value FROM poiTags WHERE id IN (SELECT tag FROM poiTags_rel WHERE poi="+q.value(0).toString()+");");
            QList<QPair<QString,QString> > tags;
            while(t.next())
                tags.append(QPair<QString,QString>(t.value(1).toString(),t.value(2).toString()));
            poiList.append(Poi(q.value(3).toDouble(),q.value(4).toDouble(),
                               q.value(5).toString(), q.value(2).toInt(),
                               tags, q.value(1).toString()));

        }
        q.exec("SELECT type,name FROM poiTypes;");
        while(q.next())
        {
            poiTypeNames.insert(q.value(0).toInt(), q.value(1).toString());
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

