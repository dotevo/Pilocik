#include "keyboardwindow.h"
#include "ui_keyboardwindow.h"
#include <QDebug>

bool KeyboardWindow::opened=0;

KeyboardWindow::KeyboardWindow(NavigationWindow *parent,QLineEdit *text,QWidget *view) :
    QFullScreenFrame(parent),
    ui(new Ui::KeyboardWindow),lastFocusedWidget(0)
{
    textEdit=text;
    ui->setupUi(this);
    ui->textFrame->layout()->addWidget(textEdit);

    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)),
            this, SLOT(saveFocusWidget(QWidget*,QWidget*)));
    signalMapper.setMapping(ui->b0_button,ui->b0_button);

    connect(ui->b0_button, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));


    connect(&signalMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(buttonClicked(QWidget*)));
    opened=true;
}

KeyboardWindow::~KeyboardWindow()
{
    delete ui;
    opened=false;
}

void KeyboardWindow::saveFocusWidget(QWidget * /*oldFocus*/, QWidget *newFocus)
{
    if (newFocus != 0 && !this->isAncestorOf(newFocus)) {
        lastFocusedWidget = newFocus;
    }
}

bool KeyboardWindow::event(QEvent *e)
{
    switch (e->type()) {
    case QEvent::WindowActivate:
        if (lastFocusedWidget)
            lastFocusedWidget->activateWindow();
        break;
    default:
        break;
    }
}


void KeyboardWindow::buttonClicked(QWidget *w){
    qDebug()<<(((QPushButton*)w)->text())<<"\n";
    //emit characterGenerated(chr);
}


