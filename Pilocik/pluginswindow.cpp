#include "pluginswindow.h"
#include "ui_pluginswindow.h"


PluginListWidget::PluginListWidget(QWidget* parent)
                : QListWidget(parent){
    setSelectionMode(QAbstractItemView::ContiguousSelection);
    for (int i = 0; i < 10; ++i){
        QListWidgetItem* item = new QListWidgetItem(QString::number(i+i));
        item->setCheckState(Qt::Unchecked);
        addItem(item);
    }
}

void PluginListWidget::mousePressEvent(QMouseEvent* event){
    QListWidgetItem* item = selectedCheckStateItem(event->pos());
    if (item)
    {
        // mouse pressed over a selected items checkbox,
        // change the check states of all selected items
        setSelectedCheckStates(item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
    }
    else
    {
        // otherwise let the base class handle mouse press
        QListWidget::mousePressEvent(event);
    }
}


void PluginListWidget::mouseReleaseEvent(QMouseEvent* event){
    // do nothing if mouse released over a selected items checkbox
    if (!selectedCheckStateItem(event->pos()))
    {
        // otherwise let the base class handle mouse release
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
    foreach (QListWidgetItem* item, selectedItems())
        item->setCheckState(checkState);
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



