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
#include "receiverrect.h"


class SimGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    SimGraphicsScene(MapGraphicsScene* mapscene, QGraphicsScene* parent=nullptr);
    void drawWalls();
    QColor gridColor;
    QString resolution;
    int ratio;
    int lengthInMeter;
    int pixelPerMeter;
    void drawRect(int i,int j);
    qreal drawRays();
    void drawScales();
    bool transmitterActivated;
    bool receiverActivated;
    bool dBmActivated;
    bool noCoActivated;
    bool colorScale = false;
    Transmitter* transmitter=nullptr;
    Receiver* receiver = nullptr;
    QColor colorRect(qreal power);
    QGraphicsTextItem * textPower;
    QGraphicsTextItem * textDebit;

signals:
    void refreshLoading(int value);
public slots:
    void setRectTransparency(int value);
    void setRectBlur(int value);
    void changeColorScale(int);
    void changeColor();
    void hidedBm(bool);
    void hideNoCo(bool);
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
private:
    int scaleMax=-20;
    int scaleMin=-55;
    QList <Ray*> rayList;
    QGraphicsTextItem*  maxScaleText;
    QList <QGraphicsItem*> scaleList;
    int pixelResolution;
    QGraphicsTextItem* receiverPower=nullptr;
    QLinearGradient* grad;
    QGraphicsRectItem *scaleRect;
    int T;
    int B;
    MapGraphicsScene* mapGraphicsScene;
    QPen gridPen;
    QPointF mirrorPointMaker(QLineF wline, QPointF initialPoint);
    std::complex<qreal> EnCalcultor(QList <Ray*> rays, QLineF mirrorToRx);
    QList <QGraphicsLineItem*> gridLines;
    QList <Wall*> wallList;
    QList <ReceiverRect*> rectList;
    Ray* ray1 = nullptr;
    bool isSameSide(Wall *w);
    std::complex<qreal> checkWalls(Ray *ray);
    qreal incidenceAngle(QLineF ray, Wall *wall);
};

#endif // SIMGRAPHICSSCENE_H
