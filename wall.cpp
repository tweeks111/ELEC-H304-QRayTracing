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
    }
    else if(material=="Bricks"){
        pen.setColor(QColor(227, 105, 11));
    }
    else if(material=="Wood"){
        pen.setColor(QColor(107, 80, 59));
    }
    pen.setWidth(1+this->thickness/2);
    setPen(pen);
    //setAcceptHoverEvents(true);
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

