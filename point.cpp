#include "point.h"

Point::Point(QGraphicsItem *parent): QGraphicsEllipseItem(parent){
    setRect(QRectF(-5, -5, 10, 10));
    pointColor=Qt::lightGray;
    QPen pen(pointColor);
    setPen(pen);
    this->setCursor(Qt::PointingHandCursor);
    setAcceptHoverEvents(true);
}

Point::Point(QPointF point,QColor pointColor, QGraphicsItem *parent): QGraphicsEllipseItem(parent)
{
    setRect(QRectF(-5, -5, 10, 10));
    setPos(point);
    QPen pen(pointColor);
    this->pointColor=pointColor;
    setPen(pen);
    this->setCursor(Qt::PointingHandCursor);
    setAcceptHoverEvents(true);
}

Point::~Point()
{

}

void Point::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    QPen pen(Qt::green);
    pen.setWidth(2);
    setPen(pen);
    QGraphicsEllipseItem::hoverEnterEvent(event);
}

void Point::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    QPen pen(pointColor);
    pen.setWidth(1);
    setPen(pen);
    QGraphicsEllipseItem::hoverLeaveEvent(event);
}
