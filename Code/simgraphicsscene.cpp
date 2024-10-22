#include "simgraphicsscene.h"

SimGraphicsScene::SimGraphicsScene(MapGraphicsScene* mapscene, QGraphicsScene* parent) : QGraphicsScene(parent)
{
    dBmActivated=false;
    this->transmitter=new Transmitter();
    this->receiver = new Receiver();
    foreach(Wall* wall,mapscene->getWalls()){
        this->wallList.push_back(new Wall(wall));
    }
    transmitter->setPos(mapscene->getTransmitter()->pos());
    transmitter->setAcceptHoverEvents(false);
    transmitter->setCursor(Qt::ArrowCursor);
    this->resolution=mapscene->resolution;
    this->pixelResolution=mapscene->pixelResolution;
    this->pixelPerMeter=mapscene->pixelPerMeter;
    this->lengthInMeter=mapscene->lengthInMeter;
    this->ratio=mapscene->ratio;
    T=200;
    B=0;
    setSceneRect(0,0,ratio*pixelResolution,pixelResolution);
    QGraphicsRectItem* whiteFontRect = new QGraphicsRectItem;
    whiteFontRect->setRect(this->sceneRect());
    whiteFontRect->setBrush(Qt::white);
    addItem(whiteFontRect);

}

void SimGraphicsScene::drawWalls()
{
    if(wallList.size()!=0){
        foreach(QGraphicsItem *wall,wallList){
            removeItem(wall);
            addItem(wall);
        }
    }

}

void SimGraphicsScene::drawRect(int i,int j){
        float frameNb = lengthInMeter/resolution.toFloat();
        float rectSize = pixelResolution/frameNb;
        receiver->setPos((j+0.5)*rectSize,(i+0.5)*rectSize);
        qreal power = drawRays();
        ReceiverRect* rect = new ReceiverRect(j*rectSize,i*rectSize,rectSize,rectSize,power);
        rect->debit();
        rectList.push_back(rect);
        rect->rectColor =colorRect(rect->power);
        QPen pen;
        pen.setColor(rect->rectColor);
        pen.setWidth(0);
        rect->setPen(pen);
        rect->setBrush(rect->rectColor);
        QGraphicsBlurEffect* blur = new QGraphicsBlurEffect;
        blur->boundingRectFor(rect->boundingRect());
        blur->setBlurHints(QGraphicsBlurEffect::PerformanceHint);
        blur->setBlurRadius(B);
        rect->setGraphicsEffect(blur);
        addItem(rect);
        update();

}

QColor SimGraphicsScene::colorRect(qreal pow)
{
    if(colorScale){
        int R=0;int G=0;
        qreal ratioPower = (pow-scaleMin)/(scaleMax-scaleMin);
        if (ratioPower >1){
            ratioPower =1;
        }
        else if(ratioPower<0){
            ratioPower=0;
        }

        if(ratioPower<0.5){
            R=255;
            G=ratioPower*2*255;
        }
        else{
            R=(1-ratioPower)*255*2;
            G=255;
        }
        return QColor(R,G,0,T);
    }
    else{
        int R=0;int G=0;int B=0;
        qreal ratioPower = (pow-scaleMin)/(scaleMax-scaleMin);
        if (ratioPower >1){
            ratioPower =1;
        }
        else if(ratioPower<0){
            ratioPower=0;
        }

        if(ratioPower<0.25){
            B=255;
            G=4*ratioPower*255;
        }
        else if(ratioPower<0.5){
            G=255;
            B=4*(0.5-ratioPower)*255;
        }
        else if(ratioPower<0.75){
            // Green -> Yellow
            G=255;
            R=4*(ratioPower-0.5)*255;
        }
        else{
            // Yellow -> Red
            R=255;
            G=4*(1-ratioPower)*255;
        }
        return QColor(R,G,B,T);
    }
}


void SimGraphicsScene::setRectTransparency(int value){
    T=value;
    foreach(ReceiverRect* rect,rectList){
        rect->rectColor.setAlpha(value);
        rect->setBrush(rect->rectColor);
        rect->setPen(rect->rectColor);
    }
    grad = new  QLinearGradient(scaleRect->rect().topLeft(),scaleRect->rect().bottomRight());
    if(colorScale){
        grad->setColorAt(0,QColor(0,255,0,value));
        grad->setColorAt(0.5,QColor(255,255,0,value));
        grad->setColorAt(1,QColor(255,0,0,value));
    }
    else{
        grad->setColorAt(0,QColor(255,0,0,value));
        grad->setColorAt(0.25,QColor(255,255,0,value));
        grad->setColorAt(0.5,QColor(0,255,0,value));
        grad->setColorAt(0.75,QColor(0,255,255,value));
        grad->setColorAt(1,QColor(0,0,255,value));
    }
    scaleRect->setBrush(*grad);
}

void SimGraphicsScene::setRectBlur(int value)
{
    B=value;
    foreach(QGraphicsRectItem* rect, rectList){
        QGraphicsBlurEffect* blur = new QGraphicsBlurEffect;
        blur->boundingRectFor(rect->rect());
        blur->setBlurHints(QGraphicsBlurEffect::PerformanceHint);
        blur->setBlurRadius(B);
        rect->setGraphicsEffect(blur);
    }
}

void SimGraphicsScene::changeColorScale(int value)
{
    this->scaleMin=-value;
    foreach(ReceiverRect* rect, rectList){
        if(noCoActivated&&rect->power<-82){
            rect->rectColor=Qt::black;
            rect->setBrush(rect->rectColor);
            rect->setPen(rect->rectColor);
        }
        else{
            rect->rectColor=colorRect(rect->power);
            rect->setBrush(rect->rectColor);
            rect->setPen(rect->rectColor);
        }
    }
    scaleRect->setBrush(*grad);
    maxScaleText->setPlainText(QString::number(-value));
}

void SimGraphicsScene::changeColor()
{
    colorScale =!colorScale;
    foreach(ReceiverRect* rect, rectList){
        if(noCoActivated&&rect->power<-82){
            rect->rectColor=Qt::black;
            rect->setBrush(rect->rectColor);
            rect->setPen(rect->rectColor);
        }
        else{
            rect->rectColor=colorRect(rect->power);
            rect->setBrush(rect->rectColor);
            rect->setPen(rect->rectColor);
        }
    }
    setRectTransparency(T);
}

void SimGraphicsScene::hidedBm(bool value)
{
    dBmActivated=value;
}

void SimGraphicsScene::hideNoCo(bool value)
{
    noCoActivated=value;
    if(noCoActivated){
        foreach(ReceiverRect* rect,rectList){
            if(rect->power<-82){
                rect->rectColor=Qt::black;
                rect->setBrush(rect->rectColor);
                rect->setPen(rect->rectColor);
            }
        }
    }
    else{
        foreach(ReceiverRect* rect, rectList){
            rect->rectColor=colorRect(rect->power);
            rect->setBrush(rect->rectColor);
            rect->setPen(rect->rectColor);
        }
    }
}


void SimGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    if(dBmActivated){
        bool mouseOutside=true;
        foreach(ReceiverRect* rect,rectList){
            if(rect->mouseOver){
                textPower->setPos(rect->rect().x()+lengthInMeter,rect->rect().y()-lengthInMeter);
                textDebit->setPos(rect->rect().x()+lengthInMeter,rect->rect().y());
                textPower->setPlainText(QString::number(rect->power,'g',4)+"dBm");
                textPower->setVisible(true);
                textDebit->setPlainText(QString::number(rect->debitBin,'g',4)+"Mb/s");
                textDebit->setVisible(true);
                mouseOutside=false;
            }
        }
        if(mouseOutside){
            textPower->setVisible(false);
            textDebit->setVisible(false);
        };
    }
    else{
        textPower->setVisible(false);
        textDebit->setVisible(false);
    }
    QGraphicsScene::mouseMoveEvent(event);
}

qreal SimGraphicsScene::drawRays()
{
        qreal power = 0;
        std::complex<qreal> En=0;
        rayList.clear();
        QLineF LineTxToRx = QLineF(transmitter->x(),transmitter->y(),receiver->x(),receiver->y());
        ray1= new Ray(LineTxToRx);
        ray1->coef*=checkWalls(ray1);
        En = EnCalcultor({ray1},LineTxToRx);
        power += (1/(8*transmitter->Ra))*pow(abs(transmitter->he*En),2);
        rayList.push_back(ray1);
        for(Wall* w1:wallList){
            //if(isSameSide(w1)){
                QPointF mirrorPoint = mirrorPointMaker(w1->line(), transmitter->pos());
                QPointF intersectPoint;
                QLineF lineRXtoMP1(receiver->x(),receiver->y(),mirrorPoint.x(),mirrorPoint.y());
                if(w1->line().intersects(lineRXtoMP1,&intersectPoint)==QLineF::BoundedIntersection){
                    Ray* ray2 = new Ray(QLineF(transmitter->x(),transmitter->y(),intersectPoint.x(),intersectPoint.y()));
                    ray2->coef*=checkWalls(ray2);
                    Ray* ray3 = new Ray(QLineF(intersectPoint.x(),intersectPoint.y(),receiver->x(),receiver->y()));
                    qreal angle = incidenceAngle(ray3->line(),w1);
                    ray3->coef*=checkWalls(ray3);
                    ray3->coef*=w1->computeRXCoef(angle);
                    rayList.push_back(ray2);
                    rayList.push_back(ray3);
                    En = EnCalcultor({ray2,ray3},lineRXtoMP1);
                    power += (1/(8*transmitter->Ra))*pow(abs(transmitter->he*En),2);
                    }
                for(Wall* w2:wallList){

                    if(w2!=w1){
                        QPointF mirrorPoint2 = mirrorPointMaker(w2->line(), mirrorPoint);
                        QPointF intersectPoint2;
                        QLineF lineRXtoMP2(receiver->x(),receiver->y(),mirrorPoint2.x(),mirrorPoint2.y());
                        if(w2->line().intersects(lineRXtoMP2,&intersectPoint2)==QLineF::BoundedIntersection){
                            QLineF lineIP2toMP1(intersectPoint2.x(),intersectPoint2.y(),mirrorPoint.x(),mirrorPoint.y());
                            QPointF intersectPoint3;
                            if(w1->line().intersects(lineIP2toMP1,&intersectPoint3)==QLineF::BoundedIntersection){
                                Ray* lineIP3toTX= new Ray(QLineF(intersectPoint3.x(),intersectPoint3.y(),transmitter->x(),transmitter->y()));
                                lineIP3toTX->coef*=checkWalls(lineIP3toTX);
                                Ray* lineRXtoIP2 = new Ray(QLineF(receiver->x(),receiver->y(),intersectPoint2.x(),intersectPoint2.y()));
                                qreal angle2 = incidenceAngle(lineRXtoIP2->line(),w2);
                                lineRXtoIP2->coef*=checkWalls(lineRXtoIP2);
                                lineRXtoIP2->coef*=w2->computeRXCoef(angle2);
                                Ray* lineIP2toIP3 = new Ray(QLineF(intersectPoint2.x(),intersectPoint2.y(),intersectPoint3.x(),intersectPoint3.y()));
                                qreal angle3 = incidenceAngle(lineIP2toIP3->line(),w1);
                                lineIP2toIP3->coef*=checkWalls(lineIP2toIP3);
                                lineIP2toIP3->coef*=w1->computeRXCoef(angle3);
                                rayList.push_back(lineIP3toTX);rayList.push_back(lineRXtoIP2);rayList.push_back(lineIP2toIP3);
                                En = EnCalcultor({lineIP3toTX,lineRXtoIP2,lineIP2toIP3},lineRXtoMP2);
                                power += (1/(8*transmitter->Ra))*pow(abs(transmitter->he*En),2);
                            }

                        }

                        for(Wall* w3:wallList){


                            if(w3!=w2){
                                QPointF mirrorPoint3 = mirrorPointMaker(w3->line(), mirrorPoint2);
                                QPointF intersectPoint4;
                                QLineF lineRXtoMP3(receiver->x(),receiver->y(),mirrorPoint3.x(),mirrorPoint3.y());
                                if(w3->line().intersects(lineRXtoMP3,&intersectPoint4)==QLineF::BoundedIntersection){
                                    QLineF lineIP4toMP2(intersectPoint4.x(),intersectPoint4.y(),mirrorPoint2.x(),mirrorPoint2.y());
                                    QPointF intersectPoint5;
                                    if(w2->line().intersects(lineIP4toMP2,&intersectPoint5)==QLineF::BoundedIntersection){
                                        QLineF lineIP5toMP1(intersectPoint5.x(),intersectPoint5.y(),mirrorPoint.x(),mirrorPoint.y());
                                        QPointF intersectPoint6;
                                        if(w1->line().intersects(lineIP5toMP1,&intersectPoint6)==QLineF::BoundedIntersection){
                                            Ray* lineIP6toTX = new Ray(QLineF(intersectPoint6.x(),intersectPoint6.y(),transmitter->x(),transmitter->y()));
                                            lineIP6toTX->coef*=checkWalls(lineIP6toTX);
                                            Ray* lineIP5toIP6 = new Ray(QLineF(intersectPoint5.x(),intersectPoint5.y(),intersectPoint6.x(),intersectPoint6.y()));
                                            qreal angle4 = incidenceAngle(lineIP5toIP6->line(),w1);
                                            lineIP5toIP6->coef*=checkWalls(lineIP5toIP6);
                                            lineIP5toIP6->coef*=w1->computeRXCoef(angle4);
                                            Ray* lineIP4toIP5 = new Ray(QLineF(intersectPoint4.x(),intersectPoint4.y(),intersectPoint5.x(),intersectPoint5.y()));
                                            qreal angle5 = incidenceAngle(lineIP4toIP5->line(),w2);
                                            lineIP4toIP5->coef*=checkWalls(lineIP4toIP5);
                                            lineIP4toIP5->coef*=w2->computeRXCoef(angle5);
                                            Ray* lineRXtoIP4 = new Ray(QLineF(receiver->x(),receiver->y(),intersectPoint4.x(),intersectPoint4.y()));
                                            qreal angle6 = incidenceAngle(lineRXtoIP4->line(),w3);
                                            lineRXtoIP4->coef*=checkWalls(lineRXtoIP4);
                                            lineRXtoIP4->coef*=w3->computeRXCoef(angle6);
                                            rayList.push_back(lineIP6toTX);rayList.push_back(lineIP5toIP6);rayList.push_back(lineIP4toIP5);rayList.push_back(lineRXtoIP4);
                                            En = EnCalcultor({lineIP6toTX,lineIP5toIP6,lineIP4toIP5,lineRXtoIP4},lineRXtoMP3);
                                            power += (1/(8*transmitter->Ra))*pow(abs(transmitter->he*En),2);

                                        }
                                    }

                                }
                            }

                        }

                    }
                }

            //}

        }
    receiver->power=10*log10(power/1e-3);
    return receiver->power;

}

void SimGraphicsScene::drawScales()
{
    if(scaleList.size()!=0){
        foreach(QGraphicsItem* item,scaleList) removeItem(item);
        scaleList.clear();
    }
    QGraphicsLineItem* scaleLine1 = new QGraphicsLineItem(pixelResolution*ratio-7*pixelPerMeter,pixelResolution-1*pixelPerMeter,pixelResolution*ratio-6*pixelPerMeter,pixelResolution-1*pixelPerMeter);
    QGraphicsLineItem* scaleLine2 = new QGraphicsLineItem(pixelResolution*ratio-7*pixelPerMeter,pixelResolution-0.8*pixelPerMeter,pixelResolution*ratio-7*pixelPerMeter,pixelResolution-1.2*pixelPerMeter);
    QGraphicsLineItem* scaleLine3 = new QGraphicsLineItem(pixelResolution*ratio-6*pixelPerMeter,pixelResolution-0.8*pixelPerMeter,pixelResolution*ratio-6*pixelPerMeter,pixelResolution-1.2*pixelPerMeter);
    QGraphicsTextItem* scaleText = new QGraphicsTextItem("1m");
    scaleList.push_back(scaleLine1);scaleList.push_back(scaleLine2);scaleList.push_back(scaleLine3);scaleList.push_back(scaleText);
    scaleLine1->setPen(QPen(Qt::gray,2));scaleLine2->setPen(QPen(Qt::gray,2));scaleLine3->setPen(QPen(Qt::gray,2));
    scaleText->setPos(pixelResolution*ratio-7.1*pixelPerMeter,pixelResolution-1*pixelPerMeter);
    scaleText->setFont(QFont("Helvetica",20));
    scaleText->setDefaultTextColor(Qt::gray);
    addItem(scaleLine1);addItem(scaleLine2);addItem(scaleLine3);addItem(scaleText);

    QGraphicsRectItem *backgroundScaleRect = new QGraphicsRectItem(pixelResolution*ratio-5*pixelPerMeter,pixelResolution-1.25*pixelPerMeter,4*pixelPerMeter,0.5*pixelPerMeter);
    scaleList.push_back(backgroundScaleRect);
    backgroundScaleRect->setBrush(Qt::white);
    addItem(backgroundScaleRect);
    scaleRect= new QGraphicsRectItem(pixelResolution*ratio-5*pixelPerMeter,pixelResolution-1.25*pixelPerMeter,4*pixelPerMeter,0.5*pixelPerMeter);
    scaleList.push_back(scaleRect);
    scaleRect->setPen(QPen(Qt::gray,2));
    grad = new QLinearGradient(scaleRect->rect().topLeft(),scaleRect->rect().bottomRight());
    if(colorScale){
        grad->setColorAt(0,QColor(0,255,0,T));
        grad->setColorAt(0.5,QColor(255,255,0,T));
        grad->setColorAt(1,QColor(255,0,0,T));
    }
    else{
        grad->setColorAt(0,QColor(255,0,0,T));
        grad->setColorAt(0.25,QColor(255,255,0,T));
        grad->setColorAt(0.5,QColor(0,255,0,T));
        grad->setColorAt(0.75,QColor(0,255,255,T));
        grad->setColorAt(1,QColor(0,0,255,T));
    }
    scaleRect->setBrush(*grad);
    addItem(scaleRect);
    QGraphicsTextItem* minScaleText = new QGraphicsTextItem();
    scaleList.push_back(minScaleText);
    minScaleText->setPlainText(QString::number(scaleMax));
    minScaleText->setScale(2);
    minScaleText->setPos(pixelResolution*ratio-5.6*pixelPerMeter,pixelResolution-pixelPerMeter);
    minScaleText->setDefaultTextColor(Qt::gray);
    maxScaleText = new QGraphicsTextItem();
    scaleList.push_back(maxScaleText);
    maxScaleText->setPlainText(QString::number(scaleMin));
    maxScaleText->setScale(2);
    maxScaleText->setPos(pixelResolution*ratio-1.7*pixelPerMeter,pixelResolution-pixelPerMeter);
    maxScaleText->setDefaultTextColor(Qt::gray);
    addItem(minScaleText);addItem(maxScaleText);

    drawWalls();
    addItem(transmitter);
    scaleList.push_back(transmitter);
    textPower = new QGraphicsTextItem;
    textDebit = new QGraphicsTextItem;
    textPower->setScale(2);
    textDebit->setScale(2);
    scaleList.push_back(textPower);
    scaleList.push_back(textDebit);
    addItem(textPower);
    addItem(textDebit);

}

QPointF SimGraphicsScene::mirrorPointMaker(QLineF wline, QPointF initialPoint)
{
    float a = (wline.p2().y()-wline.p1().y())/wline.length();
    float b = (wline.p1().x()-wline.p2().x())/wline.length();
    float c = -a*wline.p1().x()-b*wline.p1().y();
    float dist = a*initialPoint.x()+b*initialPoint.y()+c;
    QPointF mirrorPoint(initialPoint.x()-2*a*dist,initialPoint.y()-2*b*dist);
    return mirrorPoint;
}

std::complex<qreal> SimGraphicsScene::checkWalls(Ray *ray)
{
    /* Ajoute les coefficients de transmission au rayon
     * si il y a des murs sur le chemin.
     */

    QPointF intersectPoint;
    std::complex<qreal> coef=1;
    for(Wall* w1:wallList){
        if(w1->line().intersects(ray->line(),&intersectPoint)==QLineF::BoundedIntersection){
            if(intersectPoint!=ray->line().p1()&&intersectPoint!=ray->line().p2()){  //Afin d'éviter que les réflexions soient considérées commes des intersections
                qreal angle = incidenceAngle(ray->line(),w1);
                coef*=w1->computeTXCoef(angle);
            }
        }
    }
    return coef;
}

std::complex<qreal> SimGraphicsScene::EnCalcultor(QList <Ray*> rays, QLineF LineMirrorToRx)
{
    std::complex<qreal> coef = 1;
    foreach(Ray* ray,rays) coef*=ray->coef;
    //std::complex<qreal> exponent(0,-transmitter->beta0);
    qreal dn = LineMirrorToRx.length()/pixelPerMeter;
    //std::complex<qreal> En=coef*sqrt(60*transmitter->Gtx*transmitter->power)*exp(exponent*dn)/(dn);
    std::complex<qreal> En=coef*sqrt(60*transmitter->Gtx*transmitter->power)/(dn);
    return En;
}

bool SimGraphicsScene::isSameSide(Wall* w)
{
    // ATTENTION il faudra penser a prendre en compte le cas ou le RX est sur le mur

    QPointF wallP1 = w->line().p1();
    QPointF wallP2 = w->line().p2();

    bool b1 = (transmitter->x()-wallP1.x())*(wallP2.y()-wallP1.y())-(transmitter->y()-wallP1.y())*(wallP2.x()-wallP1.x())>0;
    bool b2 = (receiver->x()-wallP1.x())*(wallP2.y()-wallP1.y())-(receiver->y()-wallP1.y())*(wallP2.x()-wallP1.x())>0;

    return (b1==b2);
}

qreal SimGraphicsScene::incidenceAngle(QLineF ray,Wall* wall)
{
    qreal angle  = ray.angleTo(wall->line().normalVector());

    if(angle>270){
        angle=360-angle;
    }
    else if(angle>180){
        angle=angle-180;
    }
    else if(angle>90){
        angle=180-angle;
    }

    return angle;

}
