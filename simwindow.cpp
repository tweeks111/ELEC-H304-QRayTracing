#include "simwindow.h"
#include "simwindow.h"

SimWindow::SimWindow(MapGraphicsScene* mapscene, QWidget *parent) : QMainWindow(parent)
{
    scene = new SimGraphicsScene(mapscene);
    view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing, true);
    toolBar= new QToolBar("Tools");
    toolBar->setMovable(false);
    addToolBar(Qt::TopToolBarArea,toolBar);



    dBmBtn = new QAction("&Show dBm");
        dBmBtn->setCheckable(true);
        connect(dBmBtn,&QAction::triggered,scene,&SimGraphicsScene::hidedBm);
        dBmBtn->setIcon(QIcon("icons/dBmicon.png"));
    toolBar->addAction(dBmBtn);
    toolBar->addSeparator();

    QSlider* slider = new QSlider(Qt::Horizontal);
    slider->setMaximumWidth(100);
    slider->setMinimumWidth(50);
    this->transparency=128;
    slider->setRange(0,255);
    slider->setTickInterval(1);
    slider->setValue(128);
    connect(slider,&QSlider::valueChanged,scene,&SimGraphicsScene::setRectTransparency);

    QSlider* scaleSlider = new QSlider(Qt::Horizontal);
    scaleSlider->setMaximumWidth(100);
    scaleSlider->setMinimumWidth(50);
    scaleSlider->setRange(40,100);
    scaleSlider->setTickInterval(5);
    scaleSlider->setValue(70);
    connect(scaleSlider,&QSlider::valueChanged,scene,&SimGraphicsScene::changeColorScale);

    toolBar->addWidget(new QLabel("Scale: "));
    toolBar->addWidget(scaleSlider);
    toolBar->addSeparator();
    toolBar->addWidget(new QLabel("Transparency: "));
    toolBar->addWidget(slider);
    toolBar->addSeparator();



    loadBar = new QProgressBar;
    loadBar->setRange(0,100);
    loadBar->setValue(0);
    loadAction = toolBar->addWidget(loadBar);
    this->setCentralWidget(view);
    this->view->setRenderHint(QPainter::Antialiasing, true);
    this->view->fitInView(scene->itemsBoundingRect(),Qt::KeepAspectRatio);
    this->centralWidget()->setContentsMargins(0,0,0,0);
    menuBar=new QMenuBar(this);
    setMenuBar(menuBar);
    addMenuBar();
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(updateBar()));
    timer->start();
}

SimWindow::~SimWindow(){
}

void SimWindow::addMenuBar(){

    QMenu *menuFile= menuBar->addMenu("&File");
        QAction *saveFile = new QAction("&Export image");
        connect(saveFile,&QAction::triggered,this,&SimWindow::save);
        saveFile->setIcon(QIcon("icons/savefileicon.png"));
        saveFile->setShortcut(QKeySequence("Ctrl+S"));
        menuFile->addAction(saveFile);
        menuFile->addSeparator();

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
            QDesktopServices::openUrl(QUrl::fromLocalFile(QCoreApplication::applicationDirPath()+"/images/"));   // Not working why ?
        }

}

void SimWindow::resizeEvent(QResizeEvent *event)
{
    view->fitInView(removeMargin(scene->sceneRect(),this->view->size()),Qt::KeepAspectRatio);
    QMainWindow::resizeEvent(event);

}

void SimWindow::showEvent(QShowEvent *event)
{
    view->fitInView(removeMargin(scene->sceneRect(),this->view->size()),Qt::KeepAspectRatio);
}

void SimWindow::updateBar()
{
    qDebug("timer ok");
    if(i>scene->lengthInMeter/scene->resolution.toFloat()-1){
        timer->stop();
        toolBar->removeAction(loadAction);
        qDebug()<<i;
    }
    else{
        scene->drawRect(i,j);
        int value = i*4*scene->resolution.toFloat();
        loadBar->setValue(value);
        j++;
        if(j>scene->lengthInMeter*scene->ratio/scene->resolution.toFloat()-1){
            j=0;
            i++;
        }
    }
    scene->drawScales();
    this->view->fitInView(removeMargin(scene->sceneRect(),this->view->size()),Qt::KeepAspectRatio);
}

QRectF SimWindow::removeMargin(const QRectF& sceneRect, const QSize& viewerSize){
    const int bugMargin = 2;
    const double mx = sceneRect.width()/viewerSize.width()*bugMargin;
    const double my = sceneRect.height()/viewerSize.height()*bugMargin;
    return sceneRect.adjusted(mx, my, -mx, -my);
}
