#include "mapwindow.h"

MapWindow::MapWindow(MapGraphicsScene* scene, QGraphicsView* view,QWidget *parent): QMainWindow(parent)
{
    this->scene = scene;
    this->view = view;
    this->setCentralWidget(view);
    this->view->setRenderHint(QPainter::Antialiasing, true);
    this->centralWidget()->setContentsMargins(0,0,0,0);
    menuBar=new QMenuBar(this);
    setMenuBar(menuBar);
    addActionsToToolbar();
    addLabelsToLabelbar();
    addActionsToMenubar();

    this->scene->setMaterial(materialBox->currentText());
    this->scene->resolution=resolutionBox->currentText();
    this->show();
}

MapWindow::~MapWindow()
{
}

void MapWindow::changeThickness(int value)
{
    wallThickness->setText(" "+QString::number(value)+" cm ");
    scene->actualThickness=value;
}

void MapWindow::resizeEvent(QResizeEvent *event)
{
    view->fitInView(scene->itemsBoundingRect(),Qt::KeepAspectRatio);
    QMainWindow::resizeEvent(event);
}

void MapWindow::showEvent(QShowEvent *event)
{
    view->fitInView(scene->itemsBoundingRect(),Qt::KeepAspectRatio);
}

void MapWindow::addActionsToToolbar(){
    toolBar= new QToolBar("Tools");
    addToolBar(Qt::TopToolBarArea,toolBar);
        transmitterBtn = new QAction("&Place Transmitter");
            connect(transmitterBtn,&QAction::triggered,scene,&MapGraphicsScene::placeTransmitter);
            transmitterBtn->setIcon(QIcon("icons/TXicon.png"));
        receiverBtn = new QAction("&Place Receiver");
            connect(receiverBtn,&QAction::triggered,scene,&MapGraphicsScene::placeReceiver);
            receiverBtn->setIcon(QIcon("icons/RXicon.png"));
        hideRaysAction = new QAction("Hide Rays");
            hideRaysAction->setCheckable(true);
            connect(hideRaysAction,&QAction::toggled,scene,&MapGraphicsScene::hideRays);
            hideRaysAction->setIcon(QIcon("icons/hiderayicon.png"));
            hideRaysAction->setShortcut(QKeySequence("Ctrl+R"));
        hidePointsAction = new QAction("Hide Anchor Points");
            hidePointsAction->setCheckable(true);
            connect(hidePointsAction,&QAction::toggled,scene,&MapGraphicsScene::hidePoints);
            hidePointsAction->setIcon(QIcon("icons/hidepointsicon.png"));
            hidePointsAction->setShortcut(QKeySequence("Ctrl+P"));

        undo = new QAction("&Undo",this);
            connect(undo,&QAction::triggered,scene,&MapGraphicsScene::removeLastWall);
            undo->setShortcut(QKeySequence("Ctrl+Z"));
            undo->setIcon(QIcon("icons/undoicon.png"));
        redo = new QAction("&Redo",this);
            connect(redo,&QAction::triggered,scene,&MapGraphicsScene::addRemovedWall);
            redo->setShortcut(QKeySequence("Ctrl+Y"));
            redo->setIcon(QIcon("icons/redoicon.png"));
        clear = new QAction("Clear",this);
            connect(clear,&QAction::triggered,scene,&MapGraphicsScene::clearItems);
            clear->setShortcut(QKeySequence("Ctrl+X"));
            clear->setIcon(QIcon("icons/clearicon.png"));

        toolBar->addAction(transmitterBtn);
        toolBar->addAction(receiverBtn);
        toolBar->addSeparator();
        toolBar->addAction(hideRaysAction);
        toolBar->addAction(hidePointsAction);
        toolBar->addSeparator();
        toolBar->addAction(undo);
        toolBar->addAction(redo);
        toolBar->addSeparator();
        toolBar->addAction(clear);
        toolBar->addSeparator();

        QLabel* resolutionLabel = new QLabel("Resolution : ");
        toolBar->addWidget(resolutionLabel);
        resolutionBox=new QComboBox;
        resolutionBox->addItem(QString::number(1));resolutionBox->addItem(QString::number(0.5));resolutionBox->addItem(QString::number(0.25));
            connect(resolutionBox,&QComboBox::currentTextChanged,scene,&MapGraphicsScene::draw);
        toolBar->addWidget(resolutionBox);
        toolBar->addSeparator();
        QLabel *wallThicknessLabel= new QLabel("Wall Thichkness : ");
        QSlider* slider = new QSlider(Qt::Horizontal);
        slider->setMaximumWidth(100);
        slider->setMinimumWidth(50);
        wallThickness = new QLabel(" 15 cm");
        connect(slider,&QSlider::valueChanged,this,&MapWindow::changeThickness);
        slider->setRange(1,30);
        slider->setTickInterval(1);
        slider->setValue(15);
        toolBar->addWidget(wallThicknessLabel);
        toolBar->addWidget(slider);
        toolBar->addWidget(wallThickness);
        materialBox=new QComboBox;
        materialBox->addItem("Bricks");materialBox->addItem("Concrete");materialBox->addItem("Wood");
            connect(materialBox,&QComboBox::currentTextChanged,scene,&MapGraphicsScene::setMaterial);
        toolBar->addWidget(materialBox);
        toolBar->addSeparator();
        runBtn = new QToolButton();
        runAction = new QAction("Run Simulation");
        runAction->setIcon(QIcon("icons/runicon.png"));
        runAction->setShortcut(QKeySequence("F5"));
        runBtn->setDefaultAction(runAction);
        runBtn->setText("Run Simulation");
        runBtn->setIcon(QIcon("icons/runicon.png"));
        runBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        toolBar->addWidget(runBtn);
        toolBar->addSeparator();
}

void MapWindow::addLabelsToLabelbar(){
//Label Bar
    labelBar=new QToolBar("Labels");
    addToolBar(Qt::BottomToolBarArea,labelBar);
    positionLabel = new QLabel("x = 0 cm y = 0 cm");
        connect(scene,&MapGraphicsScene::sendPosition,positionLabel,&QLabel::setText);
    labelBar->addWidget(positionLabel);
    labelBar->addSeparator();
    lengthLabel = new QLabel("Length : 0 cm");
        connect(scene,&MapGraphicsScene::sendLength,lengthLabel,&QLabel::setText);
    labelBar->addWidget(lengthLabel);
    labelBar->addSeparator();
    distanceLabel = new QLabel("TX/RX Distance : 0 cm");
        connect(scene,&MapGraphicsScene::sendDistance,distanceLabel,&QLabel::setText);
    labelBar->addWidget(distanceLabel);
    labelBar->addSeparator();
    resolutionLabel = new QLabel(QString::number(scene->lengthInMeter*scene->ratio)+"m x "+QString::number(scene->lengthInMeter)+"m");
    labelBar->addWidget(resolutionLabel);
}

void MapWindow::addActionsToMenubar()
{
    QMenu *menuFile =menuBar->addMenu("&File");
        QAction *openFile = new QAction("&Open File");
            connect(openFile,&QAction::triggered,scene,&MapGraphicsScene::load);
        openFile->setIcon(QIcon("icons/openfileicon.png"));//QApplication::style()->standardIcon(QStyle::SP_DialogOpenButton)
        openFile->setShortcut(QKeySequence("Ctrl+O"));
        QAction *saveFile = new QAction("&Save File");
            connect(saveFile,&QAction::triggered,scene,&MapGraphicsScene::save);
        saveFile->setIcon(QIcon("icons/savefileicon.png"));//QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton
        saveFile->setShortcut(QKeySequence("Ctrl+S"));
        menuFile->addAction(openFile);
        menuFile->addAction(saveFile);
        menuFile->addSeparator();
        menuFile->addAction(runAction);

    QMenu *menuEdit= menuBar->addMenu("&Edit");
        menuEdit->addAction(undo);
        menuEdit->addAction(redo);
        menuEdit->addSeparator();
        menuEdit->addAction(hideRaysAction);
        menuEdit->addAction(hidePointsAction);
        menuEdit->addSeparator();
        menuEdit->addAction(clear);
    QMenu *menuView = menuBar->addMenu("&View");
    QMenu *menuViewModes = menuView->addMenu("Change background color");
    whiteAction = new QAction("White");
        whiteAction->setCheckable(true);
        whiteAction->setChecked(true);
        menuViewModes->addAction(whiteAction);
        connect(whiteAction,&QAction::triggered,this,&MapWindow::changeColorToWhite);
    grayAction = new QAction("Light Gray");
        grayAction->setCheckable(true);
        grayAction->setChecked(false);
        menuViewModes->addAction(grayAction);
        connect(grayAction,&QAction::triggered,this,&MapWindow::changeColorToLightgray);

}
void MapWindow::changeColorToLightgray()
{
    whiteAction->setChecked(false);
    grayAction->setChecked(true);
    scene->setGridColor(Qt::lightGray,Qt::white);

}
void MapWindow::changeColorToWhite()
{
    whiteAction->setChecked(true);
    grayAction->setChecked(false);
    scene->setGridColor(Qt::white,Qt::lightGray);
}
