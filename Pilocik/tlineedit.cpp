#include "tlineedit.h"
#include "keyboardwindow.h"
#include "navigationwindow.h"
#include <QDebug>

TLineEdit::TLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
}

void TLineEdit::focusInEvent ( QFocusEvent * e ){
//#ifdef Q_OS_WINCE_STD
    if(KeyboardWindow::opened==false){
        KeyboardWindow *kb=new KeyboardWindow(NavigationWindow::main,this,visWidget);
        kb->sizeChanged(NavigationWindow::main);
        kb->setVisible(true);
    }

//#endif
}

void TLineEdit::setVisibleWidget(QWidget*widget){
    visWidget=widget;
}
