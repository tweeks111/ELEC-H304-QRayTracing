#ifndef WALL_H
#define WALL_H

#include <QGraphicsItem>
#include <QPen>
#include <QtWidgets>
#include <complex>
class Wall : public QGraphicsLineItem{

    public:
        Wall(QGraphicsItem *parent=nullptr);
        Wall(Wall *wall);
        Wall(QLineF line,int thickness,QString material,QGraphicsItem *parent=nullptr);
        int thickness;
        qreal relPermittivity;
        qreal conductivity;
        qreal betam;
        qreal beta0;
        std::complex<qreal> Z;
        qreal Z0=376.730;

        QString material;
        bool lineActivated;
        std::complex<qreal> computeTXCoef(qreal incAngle);
        std::complex<qreal> computeRXCoef(qreal incAngle);

        /*
    protected:
        void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
        */
private:
        void computeCoef(qreal frequency);

};

#endif // WALL_H
