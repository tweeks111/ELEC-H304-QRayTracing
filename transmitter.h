#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <QGraphicsItem>
#include <QtWidgets>

class Transmitter : public QGraphicsPixmapItem
{
    public:
        Transmitter(QGraphicsItem *parent=nullptr);
        Transmitter(Transmitter* transmitter);
        int frequency=5e6;
        float power=20;
    private :
        QImage image;
        bool moving;


public slots:
        void isMoving(bool b);
};

#endif // TRANSMITTER_H
