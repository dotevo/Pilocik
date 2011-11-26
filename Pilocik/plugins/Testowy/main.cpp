#include <QtCore/QCoreApplication>
// Define plugin definitions
#include "main.h"

#include <QtPlugin>



Testowy::Testowy(){
}

void Testowy::init(){

}


void Testowy::run (){
}



Q_EXPORT_PLUGIN2(libtestowy, Testowy)







