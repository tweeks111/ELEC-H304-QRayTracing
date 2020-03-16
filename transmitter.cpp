#include "transmitter.h"

Transmitter::Transmitter(QGraphicsItem* parent) : QGraphicsPixmapItem(parent)
{
    image.load("icons/TXicon.png");
    image = image.scaled(40,40,Qt::KeepAspectRatio);
    setPixmap(QPixmap::fromImage(image));
    setOffset(-20,-20);
    setAcceptHoverEvents(true);
    moving=false;
    setCursor(Qt::OpenHandCursor);
}

Transmitter::Transmitter(Transmitter* transmitter){
    image.load("icons/TXicon.png");
    image = image.scaled(40,40,Qt::KeepAspectRatio);
    setPixmap(QPixmap::fromImage(image));
    setOffset(-20,-20);
    setAcceptHoverEvents(true);
    moving=false;
    setCursor(Qt::OpenHandCursor);
    this->setPos(transmitter->pos());
}

void Transmitter::isMoving(bool b)
{
    if(b){
        moving=true;
        setCursor(Qt::ClosedHandCursor);
    }
    else{
        moving=false;
        setCursor(Qt::OpenHandCursor);
    }
}

