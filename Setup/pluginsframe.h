#ifndef PLUGINSFRAME_H
#define PLUGINSFRAME_H

#include <QFrame>

namespace Ui {
    class PluginsFrame;
}

class PluginsFrame : public QFrame
{
    Q_OBJECT

public:
    explicit PluginsFrame(QWidget *parent = 0);
    ~PluginsFrame();

private slots:
    void init();
    void downloadFinished();
    void on_stylesList_itemSelectionChanged();
    void on_installBtn_clicked();

private:
    Ui::PluginsFrame *ui;
    int selectedIdx;
    QString selectedName;
    QString selectedFile;
};

#endif // PLUGINSFRAME_H
