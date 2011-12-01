#ifndef PLUGINSWINDOW_H
#define PLUGINSWINDOW_H

#include "qfullscreenframe.h"
#include <QApplication>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMouseEvent>
#include <QStyle>
#include <QStyleOptionViewItem>
#include <QStyleOptionButton>


class NavigationWindow;

namespace Ui {
    class PluginsWindow;
}


class PluginListWidget : public QListWidget
{
public:
    PluginListWidget(QWidget* parent = 0);

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

private:
    // returns the selected item whose checkbox lies under 'pos'
    // returns 0 if not selected, no item at pos, or does not fit inside checkbox
    QListWidgetItem* selectedCheckStateItem(const QPoint& pos);
    // sets the check state of all selected items to 'checkState'
    void setSelectedCheckStates(Qt::CheckState checkState);
};



class PluginsWindow : public QFullScreenFrame
{
    Q_OBJECT

public:
    explicit PluginsWindow(NavigationWindow *parent = 0);
    ~PluginsWindow();

private slots:
    void on_okButton_clicked();
signals:
    void closed();

private:
    Ui::PluginsWindow *ui;
};

#endif // PLUGINSWINDOW_H
