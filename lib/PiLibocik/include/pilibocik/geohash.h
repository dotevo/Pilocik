#ifndef PILIBOCIK_GEOHASH_H
#define PILIBOCIK_GEOHASH_H

#include <QString>
#include <QPair>
#include <QByteArray>
#include <QHash>

namespace PiLibocik{
    /**
     * @brief
     *
     * @class Geohash geohash.h "include/pilibocik/geohash.h"
     */
    class Geohash
    {
    private:
        static const QString char_map; /**< TODO */


        /**
         * @brief
         *
         * @fn init
         * @param geohash
         */
        void init(QString geohash);
    public:
        int geohashSize; /**< TODO */
        char *geohashValue; /**< TODO */

/**
 * @brief Empty constructor
 *
 * @fn Geohash
 */
        Geohash();
/**
 * @brief Constructor with precision and empty data.
 *
 * @fn Geohash
 * @param precision Length of geohash in base32.
 */
        Geohash(int precision);
/**
 * @brief Construct geohash from QString.
 *
 * @fn Geohash
 * @param geohash Geohash in base32.
 */
        Geohash(QString geohash);
/**
 * @brief Coping constructor.
 *
 * @fn Geohash
 * @param geohash
 */
        Geohash(const Geohash &geohash);
/**
 * @brief Geohash from position.
 *
 * @fn Geohash
 * @param lon
 * @param lat
 * @param precision
 */
        Geohash(double lon,double lat,int precision);
/**
 * @brief Geohash from data.
 *
 * @fn Geohash
 * @param n Array of chars.
 * @param size Array size.
 */
        Geohash(char *n,int size);
        /**
         * @brief Destructor
         *
         * @fn ~Geohash
         */
        ~Geohash();

        /**
         * @brief Geohash to QString (in base32).
         *
         * @fn toQString
         */
        QString toQString();
        /**
         * @brief Static method to generate geohash in position.
         *
         * @fn generateGeohash
         * @param lon
         * @param lat
         * @param precision geohash length
         */
        static QString generateGeohash(double lon, double lat, int precision);

        /**
         * @brief Increment geohash value. 009 to 00a etc.
         *
         * @fn operator ++
         * @param int
         */
        void operator++(int);
        /**
         * @brief Decrement geohash value.
         *
         * @fn operator --
         * @param int
         */
        void operator--(int);
        /**
         * @brief
         *
         * @fn operator -
         * @param q
         */
        unsigned long long  operator-(Geohash &q);
        /**
         * @brief
         *
         * @fn operator >
         * @param q
         */
        bool operator>(Geohash &q);
        /**
         * @brief
         *
         * @fn operator <
         * @param q
         */
        bool operator<(Geohash &q);
        /**
         * @brief
         *
         * @fn operator <
         * @param q
         */
        bool operator<(const Geohash &q);
        /**
         * @brief
         *
         * @fn operator ==
         * @param q
         */
        bool operator==(Geohash &q);
        /**
         * @brief
         *
         * @fn operator =
         * @param q
         */
        Geohash &operator=(Geohash &q);
        /**
         * @brief
         *
         * @fn operator =
         * @param q
         */
        Geohash &operator=(const Geohash &q);
        /**
         * @brief Return error.
         *
         * @fn getError
         * @param prec
         */
        static QPair< double, double > getError(int prec);
        /**
         * @brief Return error.
         *
         * @fn getError
         */
        QPair< double, double > getError();
    };
}

/**
 * @brief
 *
 * @fn operator <
 * @param e1
 * @param e2
 * @return bool operator
 */
inline bool operator<(const  PiLibocik::Geohash &e1, const PiLibocik::Geohash &e2){
    for(int i=0;i<e1.geohashSize&&i<e2.geohashSize;i++){
        if((short)e1.geohashValue[i]<(short)e2.geohashValue[i]){
            return true;
        }else if((short)e1.geohashValue[i]>(short)e2.geohashValue[i])
            return false;
    }
    return false;
}

/**
 * @brief Function used in Hash.
 *
 * @fn caseInsensitiveLessThan
 * @param s1
 * @param s2
 * @return bool
 */
inline bool caseInsensitiveLessThan(const PiLibocik::Geohash &s1, const PiLibocik::Geohash &s2)
{
    return s1<s2;
}


/**
 * @brief
 *
 * @fn qHash
 * @param key
 */
uint qHash(PiLibocik::Geohash &key);

#endif // GEOHASH_H
