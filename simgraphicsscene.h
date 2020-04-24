#ifndef SIMGRAPHICSSCENE_H
#define SIMGRAPHICSSCENE_H
#include "mapgraphicsscene.h"
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QFileDialog>
#include <QWidget>
#include "point.h"
#include "wall.h"
#include "transmitter.h"
#include "receiver.h"
#include "ray.h"


class SimGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    SimGraphicsScene(MapGraphicsScene* mapscene, QGraphicsScene* parent=nullptr);
    void drawWalls();
    QColor gridColor;
    QString resolution;
    void drawRect();
    void drawRays();
    bool transmitterActivated;
    bool receiverActivated;
    Transmitter* transmitter=nullptr;
    Receiver* receiver = nullptr;
public slots:
    void setRectTransparency(int value);
private:
    int ratio;
    int lengthInMeter;
    int pixelPerMeter;
    QList <Ray*> rayList;
    int pixelResolution;
    QGraphicsTextItem* receiverPower=nullptr;
    int T;
    MapGraphicsScene* mapGraphicsScene;
    QPen gridPen;
    QPointF mirrorPointMaker(QLineF wline, QPointF initialPoint);
    std::complex<qreal> EnCalcultor(QList <Ray*> rays, QLineF mirrorToRx);
    QList <QGraphicsLineItem*> gridLines;
    QList <Wall*> wallList;
    QList <QGraphicsRectItem*> rectList;
    Ray* ray1 = nullptr;
    bool raysAreHidden;
    bool isSameSide(Wall *w);
    std::complex<qreal> checkWalls(Ray *ray);
    qreal incidenceAngle(QLineF ray, Wall *wall);
};

#endif // SIMGRAPHICSSCENE_H
