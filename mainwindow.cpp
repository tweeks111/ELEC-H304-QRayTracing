#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    tabs= new QTabWidget();
    this->setCentralWidget(tabs); 
    scene = new MapGraphicsScene();
    QGraphicsView* view = new QGraphicsView(scene);

    MapWindow* mapWindow = new MapWindow(scene,view);
    tabs->addTab(mapWindow,"Map Edit");
    tabs->setTabsClosable(true);
    tabs->tabBar()->tabButton(0,QTabBar::RightSide)->resize(0,0);
    connect(mapWindow->runAction,&QAction::triggered,this,&MainWindow::addSimTab);
    connect(tabs,&QTabWidget::tabCloseRequested,this,&MainWindow::closeSimTab);


}

void MainWindow::showEvent(QShowEvent *event)
{
    this->move(QGuiApplication::primaryScreen()->geometry().center()-this->rect().center());
}

void MainWindow::addSimTab()
{
    if(!scene->isTransmitterNullptr()){
        tabsNb+=1;
        SimWindow* simWindow = new SimWindow(scene);
        tabs->addTab(simWindow,"Simulation "+QString::number(tabsNb));
        tabs->setCurrentIndex(tabs->indexOf(simWindow));
    }
    else{
        QMessageBox msgBox;
        msgBox.setText("A transmitter must be added to the scene in order to perform a simulation !");
        msgBox.setStandardButtons( QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();
    }
}

void MainWindow::closeSimTab(int index)
{
    tabs->removeTab(index);
}
