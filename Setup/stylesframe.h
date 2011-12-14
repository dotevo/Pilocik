#ifndef STYLESFRAME_H
#define STYLESFRAME_H

#include <QFrame>

namespace Ui {
    class StylesFrame;
}

class StylesFrame : public QFrame
{
    Q_OBJECT

public:
    explicit StylesFrame(QWidget *parent = 0);
    ~StylesFrame();

private slots:
    void init();
    void downloadFinished();
    void on_stylesList_itemSelectionChanged();
    void on_installBtn_clicked();

private:
    Ui::StylesFrame *ui;
    int selectedIdx;
    QString selectedName;
    QString selectedFile;
    QString selectedVersion;
};

#endif // STYLESFRAME_H
