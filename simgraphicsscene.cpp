#include "simgraphicsscene.h"

SimGraphicsScene::SimGraphicsScene(MapGraphicsScene* mapscene, QGraphicsScene* parent) : QGraphicsScene(parent)
{
    this->transmitter=new Transmitter();
    foreach(Wall* wall,mapscene->getWalls()){
        this->wallList.push_back(new Wall(wall));
    }
    transmitter->setPos(mapscene->getTransmitter()->pos());
    this->resolution=mapscene->resolution;
    this->size=1000;
    this->ratio=mapscene->ratio;
    T=128;
    setSceneRect(0,0,ratio*size,size);
    gridColor=Qt::lightGray;
    gridPen.setColor(gridColor);
    drawRect();
    drawWalls();
    addItem(transmitter);
    QGraphicsLineItem* scaleLine1 = new QGraphicsLineItem(size*ratio-2*size/20,size-2*size/20,size*ratio-size/20,size-2*size/20);
    QGraphicsLineItem* scaleLine2 = new QGraphicsLineItem(size*ratio-2*size/20,size-1.8*size/20,size*ratio-2*size/20,size-2.2*size/20);
    QGraphicsLineItem* scaleLine3 = new QGraphicsLineItem(size*ratio-size/20,size-1.8*size/20,size*ratio-size/20,size-2.2*size/20);
    QGraphicsTextItem* scaleText = new QGraphicsTextItem("1m");
    scaleText->setPos(size*ratio-2*size/20,size-2*size/20);
    scaleText->setFont(QFont("Helvetica",20));
    addItem(scaleLine1);
    addItem(scaleLine2);
    addItem(scaleLine3);
    addItem(scaleText);
}

void SimGraphicsScene::drawWalls()
{
    if(wallList.size()!=0){
        foreach(QGraphicsItem *wall,wallList){
            addItem(wall);
        }
    }

}

void SimGraphicsScene::drawRect(){
    float frameNb = 20/resolution.toFloat();
    float rectSize = size/frameNb;
    for(int j=0;j<ratio*frameNb;j++){
        for(int i =0;i<frameNb;i++){
           QGraphicsRectItem* rect = new QGraphicsRectItem(j*rectSize,i*rectSize,rectSize,rectSize);
           rectList.push_back(rect);
           rect->setPen(gridPen);
           int R = QLineF(rect->rect().center(),transmitter->pos()).length()*255/(ratio*size);
           int G = 255-QLineF(rect->rect().center(),transmitter->pos()).length()*255/(ratio*size);
           rect->setBrush(QColor(R,G,0,T));
           addItem(rect);
           update();
        }

    }
}

void SimGraphicsScene::setRectTransparency(int value){
    T=value;
    foreach(QGraphicsRectItem* rect, rectList){
        QColor color = rect->brush().color();
        color.setAlpha(value);
        rect->setBrush(color);
    }
}
