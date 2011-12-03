#ifndef NEWSFRAME_H
#define NEWSFRAME_H

#include <QFrame>
#include "filedownload.h"
#include <QDomDocument>

namespace Ui {
    class NewsFrame;
}

class NewsFrame : public QFrame
{
    Q_OBJECT

public:
    explicit NewsFrame(QWidget *parent = 0);
    ~NewsFrame();

private:
    Ui::NewsFrame *ui;
    FileDownload* news;
    QDomDocument *doc;

    void addItem(QString title, QString date, QString body);

private slots:
    void parseNewsXML(QString fileName);
};

#endif // NEWSFRAME_H
