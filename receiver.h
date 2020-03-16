#ifndef RECEIVER_H
#define RECEIVER_H

#include <QGraphicsItem>
#include <QtWidgets>

class Receiver : public QGraphicsPixmapItem
{
    public:
        Receiver(QGraphicsItem* parent=nullptr);
        Receiver(Receiver const &Receiver);
    private :
        QImage image;
        bool moving;
    public slots:
        void isMoving(bool b);
};


#endif // RECEIVER_H
