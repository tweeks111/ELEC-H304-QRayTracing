#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <QGraphicsItem>
#include <QtWidgets>

class Transmitter : public QGraphicsPixmapItem
{
    public:
        Transmitter(QGraphicsItem *parent=nullptr);
        Transmitter(Transmitter* transmitter);
        qreal frequency=5e9;
        qreal beta0=0;
        qreal power_dbm=20;
        qreal power;
        qreal Ra = 71;
        qreal he =0;
        qreal Gtx = 16/(3*3.14159);   // pas de pertes => Gtx = Dtx car rendement =1
    private :
        QImage image;
        bool moving;



public slots:
        void isMoving(bool b);
};

#endif // TRANSMITTER_H
