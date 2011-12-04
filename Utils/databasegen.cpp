#include "databasegen.h"
#include "iostream"

DatabaseGen::DatabaseGen()
{
}

void DatabaseGen::init(QString osmPath, QString outputDir, bool addIndexes)
{
    outputPath.clear();
    this->osmPath = osmPath;
    createIdx = addIndexes;
    //QString fileName = osmPath.split('\\').last();
    //fileName.truncate(fileName.size()-3);
    //fileName.append("db");
    outputPath = outputDir;
    //if(!outputDir.isEmpty()&&!outputDir.endsWith('\\'))
    //    outputPath.append('\\');
    //outputPath.append(fileName);
    qDebug()<<outputPath;
}

bool DatabaseGen::openDB()
{
    // Find QSLite driver
    db = QSqlDatabase::addDatabase("QSQLITE");

    #ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append(outputPath);
    path = QDir::toNativeSeparators(path);
    db.setDatabaseName(path);
    #else
    // NOTE: File exists in the application private folder, in Symbian Qt implementation
    db.setDatabaseName(outputPath);
    #endif

    // Open databasee
    return db.open();
}

bool DatabaseGen::deleteDB()
    {
    // Close database
    db.close();

    #ifdef Q_OS_LINUX
    // NOTE: We have to store database file into user home folder in Linux
    QString path(QDir::home().path());
    path.append(QDir::separator()).append(outputPath);
    path = QDir::toNativeSeparators(path);
    return QFile::remove(path);
    #else

    // Remove created database binary file
    return QFile::remove(outputPath);
    #endif
}

bool DatabaseGen::createTables()
{
    bool ret = false;
    if (db.isOpen())
    {
        QSqlQuery query;
        QString strQuery =
                    "CREATE TABLE osm ("
                    "version TEXT"
                    "); "
                    "CREATE TABLE nodes ("
                    "id INTEGER NOT NULL PRIMARY KEY,"
                    "lon REAL NOT NULL,"
                    "lat REAL NOT NULL,"
                    "hash TEXT NOT NULL,"
                    "hasTag INTEGER NOT NULL"
                    "); "
                    "CREATE TABLE node_tags ("
                    "ref INTEGER NOT NULL,"
                    "tag INTEGER NOT NULL"
                    "); "
                    "CREATE TABLE ways ("
                    "id INTEGER NOT NULL PRIMARY KEY,"
                    "lon1 REAL NOT NULL,"
                    "lat1 REAL NOT NULL,"
                    "lon2 REAL NOT NULL,"
                    "lat2 REAL NOT NULL"
                    "); "
                    "CREATE TABLE way_tags ("
                    "ref INTEGER NOT NULL,"
                    "tag INTEGER NOT NULL"
                    "); "
                    "CREATE TABLE way_nodes ("
                    "way INTEGER NOT NULL,"
                    "num INTEGER NOT NULL,"
                    "node INTEGER NOT NULL"
                    "); "
                    "CREATE TABLE relations ("
                    "id INTEGER NOT NULL PRIMARY KEY"
                    "); "
                    "CREATE TABLE relation_tags ("
                    "ref INTEGER NOT NULL,"
                    "tag INTEGER NOT NULL"
                    "); "
                    "CREATE TABLE members ("
                    "relation INTEGER NOT NULL,"
                    "type TEXT,"
                    "ref INTEGER NOT NULL,"
                    "role TEXT"
                    "); "
                    "CREATE TABLE tags ("
                    "id INTEGER NOT NULL PRIMARY KEY,"
                    "key TEXT,"
                    "value TEXT"
                    ");";

        foreach(QString q, strQuery.split(';'))
        {
            if(!q.isEmpty())
                ret = query.exec(q);
        }

    }
    return ret;
}

bool DatabaseGen::processXML(QString path)
{
    if (db.isOpen())
    {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            emit statusUpdate("Can't find osm map file.");
            return false;
        }
        QXmlStreamReader xml(&file);

        qint64 fileSize = file.size();

        while(!xml.atEnd() && !xml.hasError())
        {

            QXmlStreamReader::TokenType token = xml.readNext();

            if(token == QXmlStreamReader::StartDocument) {
                    continue;
            }

            if(token == QXmlStreamReader::StartElement) {

                    if(xml.name() == "osm") {
                        setVersion(xml);
                    }

                    if(xml.name() == "node") {
                        addNode(xml);
                    }

                    if(xml.name() == "way") {
                        addWay(xml);
                    }

                    if(xml.name() == "relation"){
                        addRelation(xml);
                    }
            }

            emit progressUpdate(xml.characterOffset()*100/fileSize);
        }
        file.close();
        emit progressUpdate(100);
    }
    dbInsert(true);
    return true;
}

void DatabaseGen::setVersion(QXmlStreamReader& xml)
{
    if (db.isOpen())
    {
        QSqlQuery query;
        query.exec(QString("INSERT INTO osm VALUES(%1)").arg(xml.attributes().value("version").toString()));
    }
}

void DatabaseGen::addNode(QXmlStreamReader& xml)
{
    QString id, lat, lon, hash;
    QString hasTag = "0";

    if(xml.tokenType() == QXmlStreamReader::StartElement) {
        id=xml.attributes().value("id").toString();
        lat=xml.attributes().value("lat").toString();
        lon=xml.attributes().value("lon").toString();
        hash=generateGeohash(lon.toDouble(),lat.toDouble());
    }    

    QString tagKey, tagValue;

    if(xml.tokenType() != QXmlStreamReader::EndElement){
        xml.readNextStartElement();
        if(xml.name()=="tag"){
            hasTag = "1";
            tagKey=xml.attributes().value("k").toString();
            tagValue=xml.attributes().value("v").toString();
            addNodeTag(id, tagKey, tagValue);
        }
    }

    nodesMap.insert(id.toInt(), QPair<double,double>(lon.toDouble(),lat.toDouble()));

    queriesBuffer.append(QString("INSERT INTO nodes VALUES(%1, %2, %3, '%4', %5);\n ").arg(id,lon,lat,hash,hasTag));

    dbInsert();
}

void DatabaseGen::addWay(QXmlStreamReader& xml)
{
    int num = 0;
    QString wayId = xml.attributes().value("id").toString();
    double lat1,lon1,lat2,lon2;
    bool firstNode = true;

    xml.readNextStartElement();

    while(!(xml.name()=="way" && xml.tokenType() == QXmlStreamReader::EndElement)){
        if(xml.tokenType() != QXmlStreamReader::StartElement)
        {
            xml.readNext();
            continue;
        }

        if(xml.name() == "nd")
        {
            QString ref = xml.attributes().value("ref").toString();
            queriesBuffer.append(QString("INSERT INTO way_nodes VALUES(%1, %2, %3);\n ").arg(wayId, QString::number(num), ref));
            num++;
            double lon = nodesMap[ref.toInt()].first;
            double lat = nodesMap[ref.toInt()].second;
            if(firstNode)
            {
                lat1 = lat;
                lat2 = lat;
                lon1 = lon;
                lon2 = lon;
                firstNode = false;
            }
            else
            {
                if (lon < lon1)
                    lon1 = lon;
                if (lon > lon2)
                    lon2 = lon;
                if (lat < lat1)
                    lat1 = lat;
                if (lat > lat2)
                    lat2 = lat;
            }
        }

        if(xml.name() == "tag")
        {
            QString tagKey, tagValue;
            tagKey=xml.attributes().value("k").toString();
            tagValue=xml.attributes().value("v").toString();
            addWayTag(wayId, tagKey, tagValue);
        }
        xml.readNext();
    }

    queriesBuffer.append(QString("INSERT INTO ways VALUES(%1, %2, %3, %4, %5);\n ").arg(wayId, QString::number(lon1), QString::number(lat1), QString::number(lon2), QString::number(lat2)));

    dbInsert();
}

void DatabaseGen::addRelation(QXmlStreamReader &xml)
{
    QString relId = xml.attributes().value("id").toString();
    queriesBuffer.append(QString("INSERT INTO relations VALUES(%1);\n ").arg(relId));
    xml.readNextStartElement();

    while(!(xml.name()=="relation" && xml.tokenType() == QXmlStreamReader::EndElement)){
        if(xml.tokenType() != QXmlStreamReader::StartElement)
        {
            xml.readNext();
            continue;
        }

        if(xml.name() == "member")
        {
            addMember(xml, relId);
        }

        if(xml.name() == "tag")
        {
            QString tagKey, tagValue;
            tagKey=xml.attributes().value("k").toString();
            tagValue=xml.attributes().value("v").toString();
            addRelationTag(relId, tagKey, tagValue);
        }
        xml.readNext();
    }

    dbInsert();
}

void DatabaseGen::addMember(QXmlStreamReader &xml, QString relId)
{
    QString type, ref, role;

    type = xml.attributes().value("type").toString();
    ref = xml.attributes().value("ref").toString();
    role = xml.attributes().value("role").toString();

    queriesBuffer.append(QString("INSERT INTO members VALUES(%1, '%2', %3, '%4');\n ").arg(relId, type, ref, role));
}

void DatabaseGen::addNodeTag(QString nodeId, QString tagKey, QString tagValue)
{
    int tagId = addTag(tagKey, tagValue);

    queriesBuffer.append(QString("INSERT INTO node_tags VALUES(%1, %2);\n ").arg(nodeId,QString::number(tagId)));

    dbInsert();
}

void DatabaseGen::addWayTag(QString wayId, QString tagKey, QString tagValue)
{
    int tagId = addTag(tagKey, tagValue);

    queriesBuffer.append(QString("INSERT INTO way_tags VALUES(%1, %2);\n ").arg(wayId,QString::number(tagId)));

    dbInsert();
}

void DatabaseGen::addRelationTag(QString relId, QString tagKey, QString tagValue)
{
    int tagId = addTag(tagKey, tagValue);

    queriesBuffer.append(QString("INSERT INTO relation_tags VALUES(%1, %2);\n ").arg(relId,QString::number(tagId)));

    dbInsert();
}

int DatabaseGen::addTag(QString tagKey, QString tagValue)
{
    int tagId;

    if(tagsMap.contains(tagKey+"="+tagValue))
    {
        tagId = tagsMap[tagKey+"="+tagValue];
    }
    else
    {
        tagId = tagsMap.size();
        tagsMap.insert(tagKey+"="+tagValue, tagId);
    }

    queriesBuffer.append(QString("INSERT INTO tags VALUES(%1, '%2', '%3');\n ").arg(QString::number(tagId), tagKey, tagValue));

    return tagId;
}

bool DatabaseGen::dbInsert(bool forceInsert)
{
    bool ret = false;
    if ((queriesBuffer.size() >= MAX_TRANSACTION_QUERIES || forceInsert) && db.isOpen())
    {
        QSqlQuery query;
        query.exec("BEGIN TRANSACTION;");
        foreach( QString q, queriesBuffer)
        {
            query.exec(q);
        }
        ret = query.exec("END TRANSACTION;");
        queriesBuffer.clear();
    }
    return ret;
}

QString DatabaseGen::generateGeohash(double lng, double lat)
{
    int precision = 8;
    static QString char_map =  "0123456789bcdefghjkmnpqrstuvwxyz";

    QChar hash[precision+1];

    if(lat <= 90.0 && lat >= -90.0 && lng <= 180.0 && lng >= -180.0) {

        int limiter = precision * 5.0;

        QPair<double,double> lat_interval = QPair<double,double>(-90, 90);
        QPair<double,double> lng_interval = QPair<double,double>(-180, 180);

        QPair<double,double> *interval;
        double coord, mid;
        int is_even = 1;
        unsigned int hashChar = 0;
        int i;
        for(i = 1; i <= limiter; i++) {

            if(is_even) {
                interval = &lng_interval;
                coord = lng;
            }
            else {
                interval = &lat_interval;
                coord = lat;
            }

            mid = (interval->first + interval->second) / 2.0;
            hashChar = hashChar << 1;

            if(coord > mid) {
                interval->first = mid;
                hashChar |= 0x01;
            }
            else
                interval->second = mid;

            if(!(i % 5)) {
                hash[(i - 1) / 5] = char_map.at(hashChar);
                hashChar = 0;
            }
            is_even = !is_even;
        }
    }
    return QString(hash,precision);
}

void DatabaseGen::createIndexes()
{
    QSqlQuery q;
    QString queries;
    queries =
            "CREATE INDEX IDX_NODES_ID ON nodes (id);"
            "CREATE INDEX IDX_NODES_HASH ON nodes (hash);"
            "CREATE INDEX IDX_NODES_POS ON nodes (lon,lat);"
            "CREATE INDEX IDX_WAYS_ID ON ways (id);"
            "CREATE INDEX IDX_WAYS_BOUND ON ways (lon1,lat1,lon2,lat2);"
            "CREATE INDEX IDX_TAGS_PAIR ON tags (key,value)";

    foreach(QString query,queries.split(';'))
    {
        q.exec(query);
    }
}

void DatabaseGen::run()
{
    deleteDB();
    emit statusUpdate("Opening database");
    if(!openDB()){
        emit statusUpdate("Failed to open database");
        return;
    }

    emit statusUpdate("Creating tables");
    if(!createTables()){
        emit statusUpdate("Failed to create tables");
        return;
    }

    emit statusUpdate("Processing file");
    QTime t;
    t.start();
    if(!processXML(osmPath)){
        return;
    }
    emit statusUpdate("Processed in:"+t.elapsed());

    if(createIdx){
        t.restart();
        emit statusUpdate("Creating indexes");
        createIndexes();
        emit statusUpdate("Indexes created in:" + t.elapsed());
    }
    db.close();
    emit statusUpdate("Finished");
}
