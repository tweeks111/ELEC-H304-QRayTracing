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



void ReceiverRect::debit()
{
    if(power>-51){
        debitBin=433;
    }
    else if(power<-82){
        debitBin=54;
    }
    else{
        double debitBinLog=power*(log10(433)-log10(54))/(-51+82)+4.123868;
        debitBin=(int) pow(10.0,debitBinLog);
    }
}
