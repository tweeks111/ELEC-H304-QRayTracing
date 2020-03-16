#include "receiver.h"

Receiver::Receiver(QGraphicsItem* parent): QGraphicsPixmapItem(parent)
{
    image.load("icons/RXicon.png");
    image = image.scaled(40,40,Qt::KeepAspectRatio);
    setPixmap(QPixmap::fromImage(image));
    setOffset(-20,-20);
    setAcceptHoverEvents(true);
    moving=false;
    setCursor(Qt::OpenHandCursor);
}

Receiver::Receiver(Receiver const &receiver){
    image.load("icons/RXicon.png");
    image = image.scaled(40,40,Qt::KeepAspectRatio);
    setPixmap(QPixmap::fromImage(image));
    setOffset(-20,-20);
    setAcceptHoverEvents(true);
    moving=false;
    setCursor(Qt::OpenHandCursor);
    this->setPos(receiver.pos());
}
void Receiver::isMoving(bool b)
{
    if(b){
        moving=true;
        this->setCursor(Qt::ClosedHandCursor);
    }
    else{
        moving=false;
        this->setCursor(Qt::OpenHandCursor);
    }
}
