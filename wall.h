#ifndef WALL_H
#define WALL_H

#include <QGraphicsItem>
#include <QPen>
#include <QtWidgets>

class Wall : public QGraphicsLineItem{

    public:
        Wall(QGraphicsItem *parent=nullptr);
        Wall(Wall *wall);
        Wall(QLineF line,int thickness,QString material,QGraphicsItem *parent=nullptr);
        Wall(QLineF line,int thickness,QString material,float angularVelocity,QGraphicsItem *parent=nullptr);
        int thickness;
        float permittivity;
        float conductivity;
        float beta;
        QString material;
        bool lineActivated;
        /*
    protected:
        void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
        void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
        */
private:
        float betaComputation(float angularVelocity);

};

#endif // WALL_H
