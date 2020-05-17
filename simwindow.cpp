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

    noCoBtn = new QAction("&Show no network zone");
        noCoBtn->setCheckable(true);
        connect(noCoBtn,&QAction::triggered,scene,&SimGraphicsScene::hideNoCo);
        noCoBtn->setIcon(QIcon("icons/nocoicon.png"));
    toolBar->addAction(noCoBtn);
    toolBar->addSeparator();

    QSlider* scaleSlider = new QSlider(Qt::Horizontal);
    scaleSlider->setMaximumWidth(100);
    scaleSlider->setMinimumWidth(50);
    scaleSlider->setRange(40,100);
    scaleSlider->setTickInterval(5);
    scaleSlider->setValue(55);
    connect(scaleSlider,&QSlider::valueChanged,scene,&SimGraphicsScene::changeColorScale);

    QSlider* transparencySlider = new QSlider(Qt::Horizontal);
    transparencySlider->setMaximumWidth(100);
    transparencySlider->setMinimumWidth(50);
    this->transparency=128;
    transparencySlider->setRange(0,255);
    transparencySlider->setTickInterval(1);
    transparencySlider->setValue(128);
    connect(transparencySlider,&QSlider::valueChanged,scene,&SimGraphicsScene::setRectTransparency);

    blurSlider = new QSlider(Qt::Horizontal);
    blurSlider->setMaximumWidth(100);
    blurSlider->setMinimumWidth(50);
    blurSlider->setRange(0,8);
    blurSlider->setTickInterval(1);
    connect(blurSlider,&QSlider::valueChanged,scene,&SimGraphicsScene::setRectBlur);


    toolBar->addWidget(new QLabel("Scale: "));
    toolBar->addWidget(scaleSlider);
    toolBar->addSeparator();
    toolBar->addWidget(new QLabel("Transparency: "));
    toolBar->addWidget(transparencySlider);
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
    for(int it=0;it<pow(scene->lengthInMeter/scene->resolution.toFloat(),2);it++){
        iterListI.push_back(it);
    }
    std::random_shuffle(iterListI.begin(),iterListI.end());
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
    if(iter<pow(scene->lengthInMeter/scene->resolution.toFloat(),2)){
        int i=(scene->resolution.toFloat()/scene->lengthInMeter)*iterListI.at(iter);
        int j=iterListI.at(iter);
        int res = 1/scene->resolution.toFloat();
        j = j%(scene->lengthInMeter*res);
        scene->drawRect(i,j);
        int value = 100*(iter)/(pow(scene->lengthInMeter/scene->resolution.toFloat(),2));
        loadBar->setValue(value);
        iter++;
    }
    else{
        timer->stop();
        toolBar->removeAction(loadAction);
        toolBar->addWidget(new QLabel("Blur: "));
        toolBar->addWidget(blurSlider);
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
