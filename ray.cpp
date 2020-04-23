#include "ray.h"

Ray::Ray(QGraphicsItem *parent): QGraphicsLineItem(parent)
{

}

Ray::Ray(QLineF line, QGraphicsItem *parent) : QGraphicsLineItem(parent)
{
    setLine(line);
}

Ray::Ray(QLineF line, float Pi,QGraphicsItem *parent): QGraphicsLineItem(parent)
{
    setLine(line);
    this->Po=Pi;
}

qreal Ray::transmissionAngle(qreal incAngle)
{

}

float Ray::reflexionCoef(qreal incAngle)
{

}

float Ray::transmissionCoef(qreal incAngle)
{

}
