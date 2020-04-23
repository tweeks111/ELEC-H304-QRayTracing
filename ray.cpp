#include "ray.h"

Ray::Ray(QGraphicsItem *parent): QGraphicsLineItem(parent)
{

}

Ray::Ray(QLineF line, QGraphicsItem *parent) : QGraphicsLineItem(parent)
{
    setLine(line);
}

Ray::Ray(QLineF line, float coef,QGraphicsItem *parent): QGraphicsLineItem(parent)
{
    setLine(line);
    this->coef=coef;
}

qreal Ray::transmissionAngle(qreal incAngle)
{

}

