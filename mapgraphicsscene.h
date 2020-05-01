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
    //Constructor
        MapGraphicsScene();
            using QGraphicsScene::QGraphicsScene;
    //Draw Functions
        void drawGrid(QString resolution);
        void drawWalls();
        void drawRays();
        void draw(QString resolution);
        void drawRXTX();
    //Activated Boolean
        bool lineActivated;
        bool transmitterActivated;
        bool receiverActivated;
    //Parameters
        Transmitter* transmitter=nullptr;
        Receiver* receiver = nullptr;
        QString actualMaterial;
        QPen gridPen;
        QColor pointColor;
        QColor gridColor;
        QString resolution;
        int ratio;
        int lengthInMeter;
        int pixelResolution;
        int pixelPerMeter;
        int actualThickness;
    //Get Functions
        QList<QGraphicsItem*> getItems();
        QList<Wall*> getWalls();
        Transmitter* getTransmitter();
        bool isTransmitterNullptr();
    //Set Functions
        void setGridColor(QColor background, QColor grid);
protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *event);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
private:

    //Items
        QGraphicsLineItem* tempLine = nullptr;
        QList <QGraphicsLineItem*> gridLines;
        QList <Point*> pointList;
        QList <Wall*> wallList;
        QList <Wall*> removedWalls;
        QList <Ray*> rayList;
        QGraphicsTextItem* receiverPower=nullptr;
        Ray* ray1 = nullptr;

    //Hidden Boolean
        bool raysAreHidden;
        bool pointsAreHidden;
        bool pointsAreAlwaysHidden;
    //DrawRays Functions
        bool isSameSide(Wall *w);
        std::complex<qreal> checkWalls(Ray *ray);
        std::complex<qreal> EnCalcultor(QList <Ray*> rays, QLineF mirrorToRx);
        qreal incidenceAngle(QLineF ray, Wall *wall);
        QPointF mirrorPointMaker(QLineF wline, QPointF initialPoint);



    signals:
            void sendPosition(QString);
            void sendLength(QString);
            void sendDistance(QString);
            void changeResolutionBox(QString);
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
