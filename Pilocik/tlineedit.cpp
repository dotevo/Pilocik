#include "tlineedit.h"
#include "keyboardwindow.h"
#include "navigationwindow.h"
#include <QDebug>

TLineEdit::TLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
}

void TLineEdit::mousePressEvent(QMouseEvent *){
#ifdef Q_OS_WINCE_STD
    if(KeyboardWindow::opened==false){
        KeyboardWindow *kb=new KeyboardWindow(NavigationWindow::main,visWidget);
        kb->sizeChanged(NavigationWindow::main);
        kb->setVisible(true);
        kb->textEdit->setText(this->text());
        connect(kb, SIGNAL(close(KeyboardWindow*)),
                this, SLOT(keyboardClosed(KeyboardWindow*)));
        connect(kb->textEdit,SIGNAL(textChanged (const QString)),
                this, SLOT(setText(const QString)));
        emit opened();
    }
#endif
}

void TLineEdit::setVisibleWidget(QWidget*widget){
    visWidget=widget;
}

void TLineEdit::keyboardClosed(KeyboardWindow*win){
    delete win;
    emit closed();
}
