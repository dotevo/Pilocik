#ifndef POI_H
#define POI_H

#include <QString>
#include <QList>
#include <QPair>
#include <QMap>
#include <pilibocik/point.h>

namespace PiLibocik{

    class Poi:public Point
    {
    private:
        QMap <int,QString> types;

        const QString         name;
        const int             type;
        const QList < QPair <QString, QString> >    tags;
#ifdef PiLibocik_WRITE_MODE
        const QString         geohash;
#endif
    public:
        Poi(double lon,double lat,QString name, int type, QList < QPair <QString, QString> > tags);
        QString getName() const;
        QList < QPair <QString, QString> > getTags() const;
        int getType() const;

        void setTypes(QMap <int,QString> typs);
        QString getTypeName(int type);


#ifdef PiLibocik_WRITE_MODE
        Poi(double lon,double lat,QString name, int type, QList < QPair <QString, QString> > tags, QString geohash);
        QString getGeohash();
#endif
    };
}

#endif // POI_H
