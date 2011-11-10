#include <pilibocik/geohash.h>
#include <QDebug>

int main(){
    PiLibocik::Geohash geo(4);
    for(int i=0;i<10;i++){
        geo++;
        qDebug()<<geo.toString();
    }
    return 0;
}
