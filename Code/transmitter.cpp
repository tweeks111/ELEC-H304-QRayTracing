#include "transmitter.h"

#define c 299792458
#define pi 3.1415926

Transmitter::Transmitter(QGraphicsItem* parent) : QGraphicsPixmapItem(parent)
{
    image.load("icons/TXicon.png");
    image = image.scaled(40,40,Qt::KeepAspectRatio);
    setPixmap(QPixmap::fromImage(image));
    setOffset(-20,-20);
    setAcceptHoverEvents(true);
    moving=false;
    setCursor(Qt::OpenHandCursor);
    power = pow(10,power_dbm/10)*1e-3;
    beta0=2*pi*frequency/c;
    he=-2/beta0;
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
    power = pow(10,power_dbm/10)*1e-3;
    beta0=2*pi*frequency/c;
    he=-2/beta0;
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

