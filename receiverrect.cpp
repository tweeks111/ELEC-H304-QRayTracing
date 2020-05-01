#include "receiverrect.h"

ReceiverRect::ReceiverRect(int x,int y, int w, int h,qreal power, QGraphicsItem* parent):QGraphicsRectItem(x,y,w,h,parent)
{
    this->power=power;
    setAcceptHoverEvents(true);
}


void ReceiverRect::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    mouseOver=true;
    QPen pen(Qt::cyan);
    pen.setWidth(2);
    setPen(pen);
    QGraphicsRectItem::hoverEnterEvent(event);
}

void ReceiverRect::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    mouseOver=false;
    QPen pen(rectColor);
    pen.setWidth(0);
    setPen(pen);
    QGraphicsRectItem::hoverLeaveEvent(event);
}

