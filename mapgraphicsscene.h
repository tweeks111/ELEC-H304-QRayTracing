#ifndef MAPGRAPHICSSCENE_H
#define MAPGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QFileDialog>
#include <QWidget>
#include "point.h"
#include "wall.h"
#include "transmitter.h"
#include "receiver.h"
#include "ray.h"

class MapGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
    public:
        MapGraphicsScene();
        using QGraphicsScene::QGraphicsScene;
        void drawGrid(QString resolution);
        void drawWalls();
        void drawRays();
        void draw(QString resolution);
        void drawRXTX();
        bool lineActivated;
        bool transmitterActivated;
        bool receiverActivated;
        int actualThickness;
        QString actualMaterial;
        QColor pointColor;
        QColor gridColor;
        QString resolution;
        int ratio;
        int lengthInMeter;
        int pixelResolution;
        int pixelPerMeter;
        QList<QGraphicsItem*> getItems();
        QList<Wall*> getWalls();
        Transmitter* getTransmitter();
        bool isTransmitterNullptr();
        void setGridColor(QColor background, QColor grid);
protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
private:
    int size;
    QPen gridPen;
    QPointF mirrorPointMaker(QLineF wline, QPointF initialPoint);
    std::complex<qreal> EnCalcultor(QList <Ray*> rays, QLineF mirrorToRx);
    QGraphicsLineItem* tempLine = nullptr;
    QList <QGraphicsLineItem*> gridLines;
    QList <Point*> pointList;
    QList <Wall*> wallList;
    QList <Wall*> removedWalls;
    QList <Ray*> rayList;
    Transmitter* transmitter=nullptr;
    Receiver* receiver = nullptr;
    Ray* ray1 = nullptr;
    bool raysAreHidden;
    bool pointsAreHidden;
    bool pointsAreAlwaysHidden;
    bool isSameSide(Wall *w);
    std::complex<qreal> checkWalls(Ray *ray);
    qreal incidenceAngle(QLineF ray, Wall *wall);
    signals:
            void sendPosition(QString);
            void sendLength(QString);
            void sendDistance(QString);
    public slots:
            void placeTransmitter();
            void placeReceiver();
            void removeLastWall();
            void addRemovedWall();
            void clearItems();
            void hideRays(bool);
            void hidePoints(bool);
            void setMaterial(QString);
            void save();
            void load();
};

#endif // MAPGRAPHICSSCENE_H
