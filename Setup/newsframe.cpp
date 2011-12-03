#include "newsframe.h"
#include "ui_newsframe.h"
#include "mainwindow.h"
#include <QDebug>
#include <QFrame>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QDomElement>

NewsFrame::NewsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::NewsFrame)
{
    ui->setupUi(this);

    news = new FileDownload();
    news->doDownload(QUrl("http://192.168.56.101/news.xml"));
    connect(news, SIGNAL(finished(QString)), this, SLOT(parseNewsXML(QString)));
}

NewsFrame::~NewsFrame()
{
    delete ui;
}

void NewsFrame::parseNewsXML(QString fileName)
{
    doc = new QDomDocument();
    qDebug()<<"fire!";
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Failed to open file!";
        return;
    }
    doc->setContent(&file);
    file.close();

    QDomElement item = doc->firstChildElement("news").firstChildElement("item");
    while(!item.isNull())
    {
        addItem(item.attributeNode("title").value(), item.attributeNode("date").value(), item.text().trimmed());
        item = item.nextSiblingElement("item");
    }
}

void NewsFrame::addItem(QString title, QString date, QString body)
{
    layout()->removeItem(layout()->itemAt(layout()->count()-1));
    QFrame *newsItem = new QFrame(MainWindow::main);
    QVBoxLayout *vbl = new QVBoxLayout(MainWindow::main);
    vbl->setSpacing(0);
    QLabel *titleLbl = new QLabel(title, MainWindow::main);
    titleLbl->setStyleSheet("font-weight: bold; font-size: 16px; font-family:'Trebuchet MS', Tahoma, Verdana; color: #fe7e00");
    titleLbl->setWordWrap(true);
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(0);
    effect->setColor(QColor("#FFFFFF"));
    effect->setOffset(1,1);
    titleLbl->setGraphicsEffect(effect);
    QLabel *dateLbl = new QLabel(date, MainWindow::main);
    dateLbl->setStyleSheet("font-size: 10px; font-family:'Trebuchet MS', Tahoma, Verdana;");
    dateLbl->setAlignment(Qt::AlignRight);
    QLabel *bodyLbl = new QLabel(body, MainWindow::main);
    bodyLbl->setStyleSheet("font-size: 12px; font-family:'Trebuchet MS', Tahoma, Verdana; "
                           "padding-bottom: 10px");
    bodyLbl->setWordWrap(true);
    vbl->addWidget(titleLbl);
    vbl->addWidget(bodyLbl);
    QFrame *line = new QFrame(MainWindow::main);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    vbl->addWidget(line);
    vbl->addWidget(dateLbl);
    newsItem->setLayout(vbl);
    layout()->addWidget(newsItem);
    QSpacerItem *spacer = new QSpacerItem(0, 20, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding);
    ((QBoxLayout*)layout())->addSpacerItem(spacer);
}
