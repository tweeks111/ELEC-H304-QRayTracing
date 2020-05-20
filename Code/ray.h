#ifndef RAY_H
#define RAY_H

#include <QGraphicsItem>
#include <QPen>
#include <QtWidgets>
#include "complex"

class Ray : public QGraphicsLineItem
{

public:
    Ray(QGraphicsItem *parent=nullptr);
    Ray(QLineF line, QGraphicsItem * parent = nullptr);
    Ray(QLineF line,float Pi,QGraphicsItem *parent=nullptr);
    std::complex<qreal> coef =1;
private:
    qreal transmissionAngle(qreal incAngle);
};

#endif // RAY_H
