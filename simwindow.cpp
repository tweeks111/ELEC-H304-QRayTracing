#include "simwindow.h"

SimWindow::SimWindow(MapGraphicsScene* mapscene, QWidget *parent) : QMainWindow(parent)
{
    scene = new SimGraphicsScene(mapscene);
    view = new QGraphicsView(scene);

    toolBar= new QToolBar("Tools");
    addToolBar(Qt::TopToolBarArea,toolBar);

    QSlider* slider = new QSlider(Qt::Horizontal);
    slider->setMaximumWidth(100);
    slider->setMinimumWidth(50);
    transparencyLabel = new QLabel("128");
    this->transparency=128;
    slider->setRange(0,255);
    slider->setTickInterval(1);
    slider->setValue(128);
    connect(slider,&QSlider::valueChanged,scene,&SimGraphicsScene::setRectTransparency);
    toolBar->addWidget(slider);
    toolBar->addWidget(transparencyLabel);

    this->setCentralWidget(view);
    this->view->setRenderHint(QPainter::Antialiasing, true);
    this->centralWidget()->setContentsMargins(0,0,0,0);
    menuBar=new QMenuBar(this);
    setMenuBar(menuBar);
    addMenuBar();
}

SimWindow::~SimWindow(){
}

void SimWindow::addMenuBar(){

    QMenu *menuFile= menuBar->addMenu("&File");
        QAction *saveFile = new QAction("&Export as .png");
        connect(saveFile,&QAction::triggered,this,&SimWindow::save);
        saveFile->setIcon(QIcon("icons/savefileicon.png"));
        saveFile->setShortcut(QKeySequence("Ctrl+S"));
        menuFile->addAction(saveFile);
        menuFile->addSeparator();

    QMenu *menuView = menuBar->addMenu("&View");
    QMenu *menuViewModes = menuView->addMenu("Change background color");


}

void SimWindow::save()
{
    QDateTime actualTime;
    QString date = actualTime.currentDateTime().toString("dd-MM-yyyy_hh-mm-ss");
    QImage img(scene->sceneRect().size().toSize(),QImage::Format_ARGB32);
    QPainter painter(&img);
    painter.setRenderHint(QPainter::Antialiasing);
    scene->render(&painter);

    QString fileName= QFileDialog::getSaveFileName(this, tr("Save Image"), QCoreApplication::applicationDirPath()+"/images/SIM"+date, "PNG (*.png);;BMP Files (*.bmp);;JPEG (*.JPEG)" );
        if (!fileName.isNull())
        {
            img.save(fileName);
            QDesktopServices::openUrl(QUrl::fromLocalFile(QCoreApplication::applicationDirPath()+"/images/"));
        }

}

void SimWindow::resizeEvent(QResizeEvent *event)
{
    view->fitInView(scene->itemsBoundingRect(),Qt::KeepAspectRatio);
    QMainWindow::resizeEvent(event);

}

void SimWindow::showEvent(QShowEvent *event)
{
    view->fitInView(scene->itemsBoundingRect(),Qt::KeepAspectRatio);
}
