#include "simgraphicsscene.h"

SimGraphicsScene::SimGraphicsScene(MapGraphicsScene* mapscene, QGraphicsScene* parent) : QGraphicsScene(parent)
{
    this->transmitter=new Transmitter();
    foreach(Wall* wall,mapscene->getWalls()){
        this->wallList.push_back(new Wall(wall));
    }
    transmitter->setPos(mapscene->getTransmitter()->pos());
    this->resolution=mapscene->resolution;
    this->pixelResolution=mapscene->pixelResolution;
    this->pixelPerMeter=mapscene->pixelPerMeter;
    this->lengthInMeter=mapscene->lengthInMeter;
    this->ratio=mapscene->ratio;
    T=128;
    setSceneRect(0,0,ratio*pixelResolution,pixelResolution);
    gridColor=Qt::lightGray;
    gridPen.setColor(gridColor);
    drawRect();
    drawWalls();
    addItem(transmitter);
    QGraphicsLineItem* scaleLine1 = new QGraphicsLineItem(pixelResolution*ratio-2*pixelPerMeter,pixelResolution-2*pixelPerMeter,pixelResolution*ratio-pixelPerMeter,pixelResolution-2*pixelPerMeter);
    QGraphicsLineItem* scaleLine2 = new QGraphicsLineItem(pixelResolution*ratio-2*pixelPerMeter,pixelResolution-1.8*pixelPerMeter,pixelResolution*ratio-2*pixelPerMeter,pixelResolution-2.2*pixelPerMeter);
    QGraphicsLineItem* scaleLine3 = new QGraphicsLineItem(pixelResolution*ratio-pixelPerMeter,pixelResolution-1.8*pixelPerMeter,pixelResolution*ratio-pixelPerMeter,pixelResolution-2.2*pixelPerMeter);
    QGraphicsTextItem* scaleText = new QGraphicsTextItem("1m");
    scaleText->setPos(pixelResolution*ratio-2*pixelPerMeter,pixelResolution-2*pixelPerMeter);
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
    float frameNb = lengthInMeter/resolution.toFloat();
    float rectSize = pixelResolution/frameNb;
    for(int j=0;j<ratio*frameNb;j++){
        for(int i =0;i<frameNb;i++){
           QGraphicsRectItem* rect = new QGraphicsRectItem(j*rectSize,i*rectSize,rectSize,rectSize);
           rectList.push_back(rect);
           rect->setPen(gridPen);
           int R = QLineF(rect->rect().center(),transmitter->pos()).length()*255/(ratio*pixelResolution);
           int G = 255-QLineF(rect->rect().center(),transmitter->pos()).length()*255/(ratio*pixelResolution);
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
