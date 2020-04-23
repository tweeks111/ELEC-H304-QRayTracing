#include "wall.h"

#define eps0 8.85418782e-12
#define mu0 1.25663e-6
#define pi 3.1415926
#define c 299792458

Wall::Wall(QGraphicsItem *parent) : QGraphicsLineItem(parent)
{
    QPen pen(Qt::blue);
    this->thickness=15;
    pen.setWidth(1+this->thickness/2);
    setPen(pen);
    //setAcceptHoverEvents(true);
}

Wall::Wall(QLineF line,int thickness,QString material,QGraphicsItem *parent) : QGraphicsLineItem(parent)
{
    this->thickness=thickness;
    this->material=material;
    setLine(line);
    QPen pen;
    if(material=="Concrete"){
        pen.setColor(QColor(72, 92, 99));
        this->relPermittivity=5;
        this->conductivity=0.014;
    }
    else if(material=="Bricks"){
        pen.setColor(QColor(227, 105, 11));
        this->relPermittivity=4.6;
        this->conductivity=0.02;
    }
    else if(material=="Wood"){
        pen.setColor(QColor(107, 80, 59));
        this->relPermittivity=2.25;
        this->conductivity=0.04;
    }
    pen.setWidth(1+this->thickness/2);
    setPen(pen);
    computeCoef(5e9);
    //setAcceptHoverEvents(true);
}



Wall::Wall(Wall*wall){
    this->thickness=wall->thickness;
    this->material=wall->material;
    setLine(wall->line());
    QPen pen;
    if(material=="Concrete"){
        pen.setColor(QColor(72, 92, 99));
    }
    else if(material=="Bricks"){
        pen.setColor(QColor(227, 105, 11));
    }
    else if(material=="Wood"){
        pen.setColor(QColor(107, 80, 59));
    }
    pen.setWidth(1+this->thickness/2);
    setPen(pen);


}



/*
void Wall::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    if(lineActivated){
        QPen pen(Qt::green);
        pen.setWidth(1+this->thickness/15);
        setPen(pen);
        this->setCursor(Qt::UpArrowCursor);
    }
    QGraphicsItem::hoverEnterEvent(event);
}

void Wall::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    QPen pen(Qt::blue);
    pen.setWidth(1+this->thickness/15);
    setPen(pen);
     QGraphicsItem::hoverLeaveEvent(event);
     this->setCursor(Qt::ArrowCursor);

}*/

void Wall::computeCoef(qreal frequency){

    std::complex<qreal> complexPermittivity = relPermittivity*eps0 -1i*conductivity/(2*pi*frequency);
    Z=sqrt(mu0/complexPermittivity);
    betam= 2*pi*frequency*sqrt(mu0*relPermittivity*eps0/2)*sqrt(sqrt(1+pow(conductivity/(2*pi*frequency*relPermittivity*eps0),2))-1);
    beta0= 2*pi*frequency/c;   // divided by light speed
}

std::complex<qreal> Wall::computeTXCoef(qreal incAngle)
{
    qreal incAngleRad=incAngle*pi/180;
    qreal tranAngle = asin(sqrt(1/relPermittivity)*sin(incAngleRad));  //vacuum permittivity
    qreal s = thickness/cos(tranAngle);
    std::complex<qreal> R = ((Z*cos(incAngle)-Z0*cos(tranAngle))/(Z*cos(incAngle)+Z0*cos(tranAngle)));

    std::complex<qreal> a(0,-betam*s);
    std::complex<qreal> b(0,beta0*2*s*sin(incAngle)*sin(tranAngle));
    return ((1.0-pow(R,2.0))*exp(a))/(1.0-pow(R,2)*exp(2.0*a)*exp(b));

}
std::complex<qreal> Wall::computeRXCoef(qreal incAngle)
{
    qreal incAngleRad=incAngle*pi/180;
    qreal tranAngle = asin(sqrt(1/relPermittivity)*sin(incAngleRad));  //vacuum permittivity
    qreal s = thickness/cos(tranAngle);
    std::complex<qreal> R = ((Z*cos(incAngle)-Z0*cos(tranAngle))/(Z*cos(incAngle)+Z0*cos(tranAngle)));

    std::complex<qreal> a(0,-2*betam*s);
    std::complex<qreal> b(0,beta0*2*s*sin(incAngle)*sin(tranAngle));
    return (R + (1.0-pow(R,2.0))*(R*exp(a)*exp(b))/(1.0-pow(R,2.0)*exp(a)*exp(b)));

}
