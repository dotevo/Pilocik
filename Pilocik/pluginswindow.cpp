#include "pluginswindow.h"
#include "pluginmanager.h"
#include <QPluginLoader>
#include <QFileInfo>
#include "ui_pluginswindow.h"


PluginListWidget::PluginListWidget(QWidget* parent)
                : QListWidget(parent){
    setSelectionMode(QAbstractItemView::ContiguousSelection);
    reload();
}

void PluginListWidget::reload(){
    this->clear();
    QList<QPluginLoader*> plugins=PluginManager::getInstance()->getAllPlugins();
    QListIterator<QPluginLoader*> iter(plugins);
    while(iter.hasNext()){
        QPluginLoader* a=iter.next();
        QFileInfo pathInfo( a->fileName() );
        QListWidgetItem* item = new QListWidgetItem(pathInfo.fileName());
        if(!a->isLoaded())
            item->setCheckState(Qt::Unchecked);
        else
            item->setCheckState(Qt::Checked);
        addItem(item);
    }
}

void PluginListWidget::mousePressEvent(QMouseEvent* event){
    QListWidgetItem* item = selectedCheckStateItem(event->pos());
    if (item) {
        setSelectedCheckStates(item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
    }else{
        QListWidget::mousePressEvent(event);
    }
}


void PluginListWidget::mouseReleaseEvent(QMouseEvent* event){
    if (!selectedCheckStateItem(event->pos())){
        QListWidget::mouseReleaseEvent(event);
    }
}

QListWidgetItem* PluginListWidget::selectedCheckStateItem(const QPoint& pos){
    QListWidgetItem* item = itemAt(pos);
    if (item)
    {
        // with the help of styles, check if checkbox rect contains 'pos'
        QStyleOptionButton opt;
        opt.QStyleOption::operator=(viewOptions());
        opt.rect = visualItemRect(item);
        QRect r = style()->subElementRect(QStyle::SE_ViewItemCheckIndicator, &opt);
        // assure that the item is also selected
        if (selectedItems().contains(item) && r.contains(pos))
        {
            return item;
        }
    }
    return 0;
}

void PluginListWidget::setSelectedCheckStates(Qt::CheckState checkState){
    PluginManager *a=PluginManager::getInstance();
    foreach (QListWidgetItem* item, selectedItems()){
        item->setCheckState(checkState);
        //set plugin (load or unload)
        if(checkState==Qt::Checked)
            a->loadPlugin(item->text());
        else
            a->unloadPlugin(item->text());
    }
}

//////////////////////////////////////////////////////////////////////////////////////




PluginsWindow::PluginsWindow(NavigationWindow *parent) :
    QFullScreenFrame(parent),
    ui(new Ui::PluginsWindow)
{
    ui->setupUi(this);
    sizeChanged((QWidget*)parent);
}

PluginsWindow::~PluginsWindow()
{
    delete ui;
}

void PluginsWindow::on_okButton_clicked(){
    setVisible(false);
    emit closed();
}



