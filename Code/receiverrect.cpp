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
    //Débit plafonné à 433 Mb/s
    if(power>-51){
        debitBin=433;
    }
    //Loi de relation entre le débit binaire et la puissance
    else{
        double debitBinLog=power*(log10(433)-log10(54))/(-51+82)+4.123868;
        debitBin=(int) pow(10.0,debitBinLog);
        //Débit négatif impossible
        if(debitBin<0){
            debitBin=0;
        }
    }
}
