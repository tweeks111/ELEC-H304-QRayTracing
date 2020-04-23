#include "wall.h"

Wall::Wall(QGraphicsItem *parent) : QGraphicsLineItem(parent)
{
    QPen pen(Qt::blue);
    this->thickness=15;
    pen.setWidth(1+this->thickness/2);
    setPen(pen);
    //setAcceptHoverEvents(true);
}

Wall::Wall(QLineF line,int thickness,QString material,QGraphicsItem *parent) : QGraphicsLineItem(parent)
{
    this->thickness=thickness;
    this->material=material;
    setLine(line);
    QPen pen;
    if(material=="Concrete"){
        pen.setColor(QColor(72, 92, 99));
        this->permittivity=5;
        this->conductivity=0.014;
    }
    else if(material=="Bricks"){
        pen.setColor(QColor(227, 105, 11));
        this->permittivity=4.6;
        this->conductivity=0.02;
    }
    else if(material=="Wood"){
        pen.setColor(QColor(107, 80, 59));
        this->permittivity=2.25;
        this->conductivity=0.04;
    }
    pen.setWidth(1+this->thickness/2);
    setPen(pen);
    //setAcceptHoverEvents(true);
}

Wall::Wall(QLineF line, int thickness, QString material, float angularVelocity, QGraphicsItem *parent): QGraphicsLineItem(parent)
{
    this->thickness=thickness;
    this->material=material;
    setLine(line);
    QPen pen;
    if(material=="Concrete"){
        pen.setColor(QColor(72, 92, 99));
        this->permittivity=5;
        this->conductivity=0.014;
    }
    else if(material=="Bricks"){
        pen.setColor(QColor(227, 105, 11));
        this->permittivity=4.6;
        this->conductivity=0.02;
    }
    else if(material=="Wood"){
        pen.setColor(QColor(107, 80, 59));
        this->permittivity=2.25;
        this->conductivity=0.04;
    }
    pen.setWidth(1+this->thickness/2);
    setPen(pen);
    betaComputation(angularVelocity);
}

float Wall::betaComputation(float angularVelocity)
{
    this->beta = angularVelocity*sqrt((permittivity*1.2566e-6)/2)*sqrt(sqrt(1+powf(conductivity/(angularVelocity*permittivity),2))+1);
}

Wall::Wall(Wall*wall){
    this->thickness=wall->thickness;
    this->material=wall->material;
    setLine(wall->line());
    QPen pen;
    if(material=="Concrete"){
        pen.setColor(QColor(72, 92, 99));
    }
    else if(material=="Bricks"){
        pen.setColor(QColor(227, 105, 11));
    }
    else if(material=="Wood"){
        pen.setColor(QColor(107, 80, 59));
    }
    pen.setWidth(1+this->thickness/2);
    setPen(pen);


}



/*
void Wall::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    if(lineActivated){
        QPen pen(Qt::green);
        pen.setWidth(1+this->thickness/15);
        setPen(pen);
        this->setCursor(Qt::UpArrowCursor);
    }
    QGraphicsItem::hoverEnterEvent(event);
}

void Wall::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    QPen pen(Qt::blue);
    pen.setWidth(1+this->thickness/15);
    setPen(pen);
     QGraphicsItem::hoverLeaveEvent(event);
     this->setCursor(Qt::ArrowCursor);

}*/

