#include "receiverrect.h"

ReceiverRect::ReceiverRect(int x,int y, int w, int h,qreal power, QGraphicsItem* parent):QGraphicsRectItem(x,y,w,h,parent)
{
    this->power=power;
}
