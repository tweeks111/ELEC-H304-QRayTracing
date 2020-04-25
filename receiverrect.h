#ifndef RECEIVERRECT_H
#define RECEIVERRECT_H

#include <QGraphicsItem>
#include <QPen>
#include <QtWidgets>
#include <complex>

class ReceiverRect : public QGraphicsRectItem
{
public:
    ReceiverRect(int x,int y, int w, int h,qreal power, QGraphicsItem* parent=nullptr);
    qreal power;
};

#endif // RECEIVERRECT_H
