#ifndef RAY_H
#define RAY_H

#include <QGraphicsItem>
#include <QPen>
#include <QtWidgets>

class Ray : public QGraphicsLineItem
{

public:
    Ray(QGraphicsItem *parent=nullptr);
    Ray(QLineF line, QGraphicsItem * parent = nullptr);
    Ray(QLineF line,float Pi,QGraphicsItem *parent=nullptr);
    float Po;
private:
    qreal transmissionAngle(qreal incAngle);
    float reflexionCoef(qreal incAngle);
    float transmissionCoef(qreal incAngle);
};

#endif // RAY_H
