#ifndef POINT_H
#define POINT_H

#include <QGraphicsItem>
#include <QPen>
#include <QtWidgets>

class Point: public QGraphicsEllipseItem{
public:
    Point(QGraphicsItem *parent=nullptr);
    Point(QPointF point, QColor pointColor, QGraphicsItem *parent=nullptr);
    ~Point();
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
private:
    QColor pointColor;
};

#endif // POINT_H
