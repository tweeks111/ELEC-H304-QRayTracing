#ifndef SIMGRAPHICSSCENE_H
#define SIMGRAPHICSSCENE_H
#include <QGraphicsScene>
#include "mapgraphicsscene.h"
#include "wall.h"
#include "transmitter.h"


class SimGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    SimGraphicsScene(MapGraphicsScene* mapscene, QGraphicsScene* parent=nullptr);
    void drawWalls();
    QColor gridColor;
    QString resolution;
    void drawRect();
public slots:
    void setRectTransparency(int value);
private:
    int size;
    int ratio;
    int T;
    QPen gridPen;
    QList <QGraphicsLineItem*> gridLines;
    QList <Wall*> wallList;
    QList <QGraphicsRectItem*> rectList;
    Transmitter* transmitter;
};

#endif // SIMGRAPHICSSCENE_H
