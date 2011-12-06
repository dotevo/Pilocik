#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newsframe.h"
#include "installframe.h"
#include "mapdownloadframe.h"
#include "resourcesmanager.h"
#include <QTimer>
#include <QMouseEvent>
#include <QDebug>

MainWindow *MainWindow::main=0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    isDraging = false;

    ui->setupUi(this);
    resize(QSize(800,600));
    Qt::WindowFlags flags = Qt::FramelessWindowHint;
    setWindowFlags(flags);
    setAttribute(Qt::WA_TranslucentBackground);
    tabs.append(ui->tab1);
    tabs.append(ui->tab2);
    tabs.append(ui->tab3);
    tabs.append(ui->tab4);
    tabs.append(ui->tab5);
    tabs.append(ui->tab6);

    NewsFrame *nframe = new NewsFrame();
    ui->contentFrame->layout()->addWidget(nframe);
    tabFrames.append(nframe);
    MapDownloadFrame *mdframe = new MapDownloadFrame;
    mdframe->hide();
    tabFrames.append(mdframe);
    ui->contentFrame->layout()->addWidget(mdframe);
    InstallFrame *iframe = new InstallFrame();
    iframe->hide();
    tabFrames.append(iframe);
    ui->contentFrame->layout()->addWidget(iframe);

    activeTab = 0;
    selectTab(0);

    asc = ActiveSyncComm::getInstance(qApp);
    rm = ResourcesManager::getInstance(qApp);
    connect(asc, SIGNAL(connected()), rm, SLOT(getDeviceResources()));
    asc->reconnect();

    connect(rm, SIGNAL(serverResObtained()), mdframe, SLOT(init()));
    connect(rm, SIGNAL(localResObtained()), iframe, SLOT(init()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::hideAllTabs()
{
    foreach(QFrame *el, tabFrames)
        el->hide();
}

void MainWindow::selectTab(int id){
    hideAllTabs();
    tabFrames.at(id)->show();
    QString activeTabStyle = "QPushButton{width:173px;height:65px;padding: 0;padding-left: 15px;font-size: 16px;"
            "font-family: 'Trebuchet MS', Tahoma, Verdana;font-weight: bold;color: #e37201;"
            "text-align: left;background-image: url(':/images/activeTab.png');background-repeat: no-repeat;"
            "background-color: transparent;background-position: right center;}";
    QString inactiveTabStyle = "QPushButton{width:173px;height:65px;padding: 0;padding-left: 15px;font-size: 16px;"
            "font-family: 'Trebuchet MS', Tahoma, Verdana;font-weight: bold;color: rgb(255, 255, 255);"
            "text-align: left;background-image: url(':/images/inactiveTab.png');background-repeat: no-repeat;"
            "background-color: transparent;background-position: right center;}";

    tabs[activeTab]->setStyleSheet(inactiveTabStyle);
    tabs[id]->setStyleSheet(activeTabStyle);
    activeTab = id;
}

void MainWindow::showEvent(QShowEvent *event)
{
    QApplication::postEvent(this, new QEvent(QEvent::UpdateRequest), Qt::LowEventPriority);
    QWidget::showEvent(event);
}

void MainWindow::on_tab1_clicked(){
    selectTab(0);
}

void MainWindow::on_tab2_clicked(){
    selectTab(1);
}

void MainWindow::on_tab3_clicked(){
    selectTab(2);
}

void MainWindow::on_tab4_clicked(){
    selectTab(3);
}

void MainWindow::on_tab5_clicked(){
    selectTab(4);
}

void MainWindow::on_tab6_clicked(){
    selectTab(5);
}

void MainWindow::on_exitBtn_clicked(){
    setVisible(false);
    this->close();
}

void MainWindow::on_minimalizeBtn_clicked(){
    showMinimized();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton &&
            (event->globalY() - geometry().y()) < 103 &&
            (event->globalX() - geometry().x()) < 692) {
        isDraging = true;
        dragPosition = event->globalPos() - QPoint(geometry().x(), geometry().y());
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (isDraging && event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    isDraging = false;
}


