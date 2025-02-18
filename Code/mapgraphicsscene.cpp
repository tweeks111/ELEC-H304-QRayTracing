#include "mapgraphicsscene.h"

MapGraphicsScene::MapGraphicsScene(){
    this->pixelResolution=1000;
    this->ratio=1;
    this->lengthInMeter=25;
    this->pixelPerMeter=pixelResolution/lengthInMeter;
    setSceneRect(0,0,ratio*pixelResolution,pixelResolution);
    gridColor=Qt::lightGray;
    pointColor=Qt::lightGray;
    gridPen.setColor(gridColor);
    setBackgroundBrush(QBrush(Qt::white));
    drawGrid("1");
    lineActivated=false;
    transmitterActivated=false;
    receiverActivated=false;
    raysAreHidden=false;
    pointsAreHidden=false;
    pointsAreAlwaysHidden=false;
    actualThickness=15;
    actualMaterial="Concrete";
    resolution="1";


}

void MapGraphicsScene::drawGrid(QString resolution)
{
    foreach(QGraphicsItem *line,gridLines) removeItem(line);
    foreach(QGraphicsItem *point,pointList) removeItem(point);
    gridLines.clear();
    pointList.clear();
    QList <QGraphicsLineItem*> tempList;
    float frameNb = lengthInMeter/resolution.toFloat();
    for(int i =0;i<=frameNb;i++){
       QGraphicsLineItem* line = new QGraphicsLineItem(0,i*pixelResolution/frameNb,ratio*pixelResolution,i*pixelResolution/frameNb);
       line->setPen(gridPen);
       gridLines.push_back(line);
       tempList.push_back(line);
       addItem(line);
    }
    for(int i =0;i<=ratio*frameNb;i++){
       QGraphicsLineItem* line = new QGraphicsLineItem(i*pixelResolution/frameNb,0,i*pixelResolution/frameNb,pixelResolution);
       line->setPen(gridPen);
       gridLines.push_back(line);
       addItem(line);
       if(!pointsAreHidden&&!pointsAreAlwaysHidden){
           foreach(QGraphicsLineItem *lineItem,tempList){
               QPointF* intersecPoint = new QPointF();
               int anyway = line->line().intersects(lineItem->line(),intersecPoint);
               Point *ip = new Point(*intersecPoint,pointColor);
               pointList.push_back(ip);
               this->addItem(ip);
           }

        }
    }

}

void MapGraphicsScene::drawWalls()
{
    if(wallList.size()!=0){
        foreach(QGraphicsItem *wall,wallList){
            removeItem(wall);
            addItem(wall);
        }
    }

}

void MapGraphicsScene::draw(QString resolution)
{
    this->resolution=resolution;
    drawGrid(resolution);
    drawWalls();
    drawRXTX();
}

void MapGraphicsScene::drawRXTX()
{
    if(transmitter&&receiver&&!raysAreHidden){
        drawRays();

    }
    if(transmitter){
        removeItem(transmitter);
        addItem(transmitter);
     }
    if(receiver){
        removeItem(receiver);
        addItem(receiver);
    }
}

bool MapGraphicsScene::isTransmitterNullptr()
{
    return (transmitter==nullptr);
}

QList<QGraphicsItem*> MapGraphicsScene::getItems()
{
    QList<QGraphicsItem*> list;
    foreach(Wall *wall,wallList){
        QGraphicsItem *copiedItem=wall;
        list.push_back(copiedItem);
    }
    QGraphicsItem *copiedTX=transmitter;
    list.push_back(copiedTX);
    QGraphicsItem *copiedRX=receiver;
    list.push_back(copiedRX);
    return list;
}

QList<Wall *> MapGraphicsScene::getWalls()
{
    return wallList;
}

Transmitter* MapGraphicsScene::getTransmitter()
{
    return transmitter;
}

void MapGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        if(transmitterActivated){
            transmitter->setPos(event->scenePos().x(),event->scenePos().y());
            transmitterActivated=false;
            transmitter->isMoving(false);
            pointsAreHidden=false;

        }
        else if(receiverActivated){
            receiver->setPos(event->scenePos().x(),event->scenePos().y());
            receiverActivated=false;
            receiver->isMoving(false);
            pointsAreHidden=false;;
        }
        else{
            if(transmitter){
                if(transmitter->isUnderMouse()){
                    transmitterActivated=true;
                    transmitter->isMoving(true);
                    pointsAreHidden=true;
                }
            }
            if(receiver){
                if(receiver->isUnderMouse()){
                    receiverActivated=true;
                    receiver->isMoving(true);
                    pointsAreHidden=true;
                }
            }
            if(!transmitterActivated&&!receiverActivated){
                for (Point* p:pointList) {
                    if (p->isUnderMouse()){
                        if(!lineActivated){
                            if(!tempLine){
                                tempLine = new QGraphicsLineItem;
                            }
                            addItem(tempLine);
                            QLineF l(p->pos(),p->pos());
                            tempLine->setLine(l);
                            lineActivated=true;
                        }
                        else if(tempLine->line().p1()!=p->pos()){
                            Wall* wall = new Wall(QLineF(tempLine->line().p1(), p->pos()),actualThickness,actualMaterial);
                            wallList.push_back(wall);
                            addItem(wall);
                            this->removeItem(tempLine);
                            lineActivated=false;
                            emit sendLength("Length : "+QString::number(wall->line().length()/pixelPerMeter)+" m ");
                        }
                        break;
                    }

                }
            }

        }
        draw(resolution);
    }
    else if(event->button()==Qt::RightButton){
        if(lineActivated){
            removeItem(tempLine);
            lineActivated=false;
        }

    }
}

void MapGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(lineActivated){
        QLineF l(tempLine->line().p1(), event->scenePos());
        tempLine->setLine(l);
        emit sendLength("Length : "+QString::number(tempLine->line().length()/pixelPerMeter)+" m ");

    }
    if(transmitterActivated){
        transmitter->setPos(event->scenePos().x(),event->scenePos().y());
        if(receiver){
            drawRXTX();
            receiverPower->setPlainText(QString::number(receiver->power)+" dBm");
            receiverDebit->setPlainText(QString::number(receiver->debit)+" Mb/s");
        }
    }
    if(receiverActivated){
        receiver->setPos(event->scenePos().x(),event->scenePos().y());
        receiverPower->setPlainText(QString::number(receiver->power)+" dBm");
        receiverPower->setPos(event->scenePos().x()+pixelPerMeter,event->scenePos().y()-pixelPerMeter);
        receiverDebit->setPlainText(QString::number(receiver->debit)+" Mb/s");
        receiverDebit->setPos(event->scenePos().x()+pixelPerMeter,event->scenePos().y());
        if(transmitter){
            drawRXTX();
        }
    }
    if(receiver&&transmitter){
        sendDistance("TX/RX Distance : "+QString::number(ray1->line().length()/pixelPerMeter)+" m");
    }
    sendPosition("x : "+QString::number(event->scenePos().x()/pixelPerMeter)+" m y : "+QString::number(event->scenePos().y()/pixelPerMeter)+" m ");
    QGraphicsScene::mouseMoveEvent(event);
}

void MapGraphicsScene::placeTransmitter()
{
    if(transmitter){
        transmitterActivated=false;
        removeItem(transmitter);
        transmitter=nullptr;
        if(rayList.size()!=0){
            foreach(QGraphicsLineItem* ray,rayList) removeItem(ray);
            rayList.clear();
        }
        pointsAreHidden=false;
        draw(resolution);
    }
    else{
        receiverActivated=false;
        transmitterActivated=true;
        pointsAreHidden=true;
        transmitter=new Transmitter();
        transmitter->isMoving(true);
        addItem(transmitter);
        draw(resolution);
    }
}

void MapGraphicsScene::placeReceiver()
{
    if(receiver){
        receiverActivated=false;
        removeItem(receiver);
        removeItem(receiverPower);
        removeItem(receiverDebit);
        receiver=nullptr;
        if(rayList.size()!=0){
            foreach(QGraphicsLineItem* ray,rayList) removeItem(ray);
            rayList.clear();
        }
        pointsAreHidden=false;
        draw(resolution);
    }
    else{
        transmitterActivated=false;
        receiverActivated=true;
        receiver=new Receiver();
        receiverPower=new QGraphicsTextItem;
        receiverPower->setScale(2);
        receiverPower->setPlainText(QString::number(receiver->power)+" dBm");
        receiverPower->setPos(receiver->x()+pixelPerMeter,receiver->y()-pixelPerMeter);
        receiverDebit=new QGraphicsTextItem;
        receiverDebit->setScale(2);
        receiverDebit->setPlainText(QString::number(receiver->debit)+" Mb/s");
        receiverDebit->setPos(receiver->x()+pixelPerMeter,receiver->y());
        pointsAreHidden=true;
        addItem(receiver);
        addItem(receiverPower);
        addItem(receiverDebit);
        draw(resolution);
        receiver->isMoving(true);
    }
}

void MapGraphicsScene::removeLastWall()
{
    if(wallList.size()!=0){
        removeItem(wallList.last());
        removedWalls.push_back(wallList.last());
        wallList.removeLast();
        drawWalls();
        if(transmitter&&receiver){
            drawRays();
        }
    }
}

void MapGraphicsScene::addRemovedWall()
{
    if(removedWalls.size()!=0){
        wallList.push_back(removedWalls.last());
        removedWalls.removeLast();
        addItem(wallList.last());
        if(transmitter&&receiver){
            drawRays();
        }
    }
}

void MapGraphicsScene::clearItems()
{
    if(wallList.size()!=0){
        foreach(Wall* wall,wallList) removeItem(wall);
        wallList.clear();
    }
    if(transmitter){ removeItem(transmitter);transmitter=nullptr;}
    if(receiver){ removeItem(receiver);receiver=nullptr;removeItem(receiverPower);removeItem(receiverDebit);}
    if(rayList.size()!=0){
        foreach(QGraphicsLineItem* ray,rayList) removeItem(ray);
        rayList.clear();
    }
}

void MapGraphicsScene::hideRays(bool checked)
{
    if(!checked){
        raysAreHidden=false;
        if(transmitter&&receiver){
            drawRays();
        }
    }
    else{
        raysAreHidden=true;
        foreach(QGraphicsLineItem* ray,rayList) removeItem(ray);
        rayList.clear();
    }
}

void MapGraphicsScene::hidePoints(bool checked)
{
    pointsAreAlwaysHidden=checked;
    draw(resolution);

}

void MapGraphicsScene::setMaterial(QString material)
{
    this->actualMaterial=material;
}

void MapGraphicsScene::setGridColor(QColor background,QColor grid)
{
    gridColor=grid;
    pointColor=grid;
    gridPen.setColor(gridColor);
    setBackgroundBrush(QBrush(background));
    draw(resolution);
}

void MapGraphicsScene::save()
{
    QDateTime actualTime;
    QString date = actualTime.currentDateTime().toString("dd-MM-yyyy_hh-mm-ss");
    QString fileName= QFileDialog::getSaveFileName(nullptr, tr("Open File"), QCoreApplication::applicationDirPath()+"/saves/MAP"+date, "Ray File (*.ray)" );
    QFile file;
    if (!fileName.isNull())
        {
            file.setFileName(fileName);
            file.open(QIODevice::WriteOnly);
            QDataStream out(&file);
            out << resolution;
            out << wallList.size();
            foreach(Wall* wall, wallList){
                out << wall->line();
                out << wall->thickness;
                out << wall->material;
            }
            if(transmitter){
                out << (bool) true;
                out << transmitter->pos();
            }
            else{
                out<< (bool) false;
            }
            if(receiver){

                out << (bool) true;
                out << receiver->pos();
            }
            else{
                out<<(bool) false;
            }
            file.close();
        }

}

void MapGraphicsScene::load()
{
    bool stop=false;
    if(wallList.size()!=0){
        QMessageBox msgBox;
        msgBox.setText("The project has been modified.");
        msgBox.setInformativeText("Do you want to save your changes ?");
        msgBox.setStandardButtons( QMessageBox::Cancel| QMessageBox::Discard | QMessageBox::Save);
        msgBox.setDefaultButton(QMessageBox::Save);
        msgBox.setIcon(QMessageBox::Warning);
        int ret = msgBox.exec();
        switch(ret){
            case QMessageBox::Save:
                save();
                stop=false;
                break;
            case QMessageBox::Discard:
                stop=false;
                break;
            case QMessageBox::Cancel:
                stop=true;
                break;
        }
    }
    if(!stop){
        QString fileName = QFileDialog::getOpenFileName(nullptr,
                                                        tr("Open File"),QCoreApplication::applicationDirPath()+"/saves/",
                                                        tr("Ray File (*.ray)"));
        if (fileName.isEmpty())
            return;
        else {
            QFile file(fileName);
            if (!file.open(QIODevice::ReadOnly)) {
                QMessageBox::information(nullptr, tr("Unable to open file"),
                    file.errorString());
                return;
            }
            clearItems();
            QDataStream in(&file);
            in.setVersion(QDataStream::Qt_5_14);
            QString resolution;
            in >> resolution;
            emit changeResolutionBox(resolution);
            int wallNb;
            in >> wallNb;

            for(int i=0;i<wallNb;i++){
                QLineF line;
                int thickness;
                QString material;
                in >> line >> thickness >> material;
                Wall* wall =new Wall(line,thickness,material);
                wallList.push_back(wall);
                addItem(wall);
            }
            bool b;
            in >> b;
            if(b){
                transmitter=new Transmitter();
                QPointF pos;
                in>>pos;
                transmitter->setPos(pos);
                addItem(transmitter);
            }
            in>>b;
            if(b){
                receiver=new Receiver();
                QPointF pos;
                in>>pos;
                receiver->setPos(pos);
                addItem(receiver);
                receiverPower=new QGraphicsTextItem;
                receiverPower->setScale(2);
                receiverPower->setPlainText(QString::number(receiver->power)+" dBm");
                receiverPower->setPos(receiver->x()+pixelPerMeter,receiver->y()-pixelPerMeter);
                addItem(receiverPower);

                receiverDebit=new QGraphicsTextItem;
                receiverDebit->setScale(2);
                receiverDebit->setPlainText(QString::number(receiver->debit)+" Mb/s");
                receiverDebit->setPos(receiver->x()+pixelPerMeter,receiver->y());
                addItem(receiverDebit);
            }
        file.close();
        draw(resolution);
        }
    }

}

QPointF MapGraphicsScene::mirrorPointMaker(QLineF wline, QPointF initialPoint)
{
    float a = (wline.p2().y()-wline.p1().y())/wline.length();
    float b = (wline.p1().x()-wline.p2().x())/wline.length();
    float c = -a*wline.p1().x()-b*wline.p1().y();
    float dist = a*initialPoint.x()+b*initialPoint.y()+c;
    QPointF mirrorPoint(initialPoint.x()-2*a*dist,initialPoint.y()-2*b*dist);
    return mirrorPoint;
}

std::complex<qreal> MapGraphicsScene::checkWalls(Ray *ray)
{
    /* Ajoute les coefficients de transmission au rayon
     * si il y a des murs sur le chemin.
     */

    QPointF intersectPoint;
    std::complex<qreal> coef=1;
    for(Wall* w:wallList){
        if(w->line().intersects(ray->line(),&intersectPoint)==QLineF::BoundedIntersection){
            if(intersectPoint!=ray->line().p1()&&intersectPoint!=ray->line().p2()){  //Afin d'éviter que les réflexions soient considérées commes des intersections
                qreal angle = incidenceAngle(ray->line(),w);
                coef*=w->computeTXCoef(angle);
            }
        }
    }
    return coef;
}

std::complex<qreal> MapGraphicsScene::EnCalcultor(QList <Ray*> rays, QLineF LineMirrorToRx)
{
    std::complex<qreal> coef = 1;
    foreach(Ray* ray,rays){
        coef*=ray->coef;
        //qDebug()<<"coef :" +QString::number(real(coef))+"+"+QString::number(imag(coef))+"j";
    }
    //qDebug()<<"Coef : "+QString::number(real(coef))+"+"+QString::number(imag(coef))+"j";
    std::complex<qreal> exponent(0,-transmitter->beta0);
    qreal dn = LineMirrorToRx.length()/pixelPerMeter;
    //qDebug()<< "dn : " +QString::number(dn);
    std::complex<qreal> En=coef*sqrt(60*transmitter->Gtx*transmitter->power)*exp(exponent*dn)/(dn);
    //qDebug()<< "En : "+ QString::number(real(En))+ "+" +QString::number(imag(En))+"j";
    return En;
}

void MapGraphicsScene::drawRays()
{
    if(!raysAreHidden){
        qreal power = 0;
        std::complex<qreal> En=0;
        if(rayList.size()!=0){
            foreach(Ray* ray, rayList) removeItem(ray);
            rayList.clear();};
        QPen rayPen(QColor(106, 224, 27));
        QLineF LineTxToRx = QLineF(transmitter->x(),transmitter->y(),receiver->x(),receiver->y());
        ray1= new Ray(LineTxToRx);
        ray1->coef*=checkWalls(ray1);
        En = EnCalcultor({ray1},LineTxToRx);
        power += (1/(8*transmitter->Ra))*pow(abs(transmitter->he*En),2);
        ray1->setPen(rayPen);
        rayList.push_back(ray1);
        addItem(ray1);
        for(Wall* w1:wallList){
            // 1 REFLEXION
            //if(isSameSide(w1)){
                QPointF mirrorPoint = mirrorPointMaker(w1->line(), transmitter->pos());
                QPointF intersectPoint;
                QLineF lineRXtoMP1(receiver->x(),receiver->y(),mirrorPoint.x(),mirrorPoint.y());
                if(w1->line().intersects(lineRXtoMP1,&intersectPoint)==QLineF::BoundedIntersection){
                    rayPen.setColor(QColor(224, 221, 27));
                    Ray* ray2 = new Ray(QLineF(transmitter->x(),transmitter->y(),intersectPoint.x(),intersectPoint.y()));
                    ray2->coef*=checkWalls(ray2);
                    Ray* ray3 = new Ray(QLineF(intersectPoint.x(),intersectPoint.y(),receiver->x(),receiver->y()));
                    qreal angle = incidenceAngle(ray3->line(),w1);
                    ray3->coef*=checkWalls(ray3);
                    ray3->coef*=w1->computeRXCoef(angle);
                    rayList.push_back(ray2);
                    rayList.push_back(ray3);
                    ray2->setPen(rayPen);
                    ray3->setPen(rayPen);
                    En = EnCalcultor({ray2,ray3},lineRXtoMP1);
                    power += (1/(8*transmitter->Ra))*pow(abs(transmitter->he*En),2);
                    addItem(ray2);addItem(ray3);
                    }
                for(Wall* w2:wallList){
                    //2 REFLEXIONS
                    if(w2!=w1){
                        rayPen.setColor(QColor(224, 152, 27));
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
                                lineIP3toTX->setPen(rayPen);lineRXtoIP2->setPen(rayPen);lineIP2toIP3->setPen(rayPen);
                                En = EnCalcultor({lineIP3toTX,lineRXtoIP2,lineIP2toIP3},lineRXtoMP2);
                                power += (1/(8*transmitter->Ra))*pow(abs(transmitter->he*En),2);
                                addItem(lineIP3toTX);addItem(lineRXtoIP2);addItem(lineIP2toIP3);
                            }

                        }

                        for(Wall* w3:wallList){
                            //3 REFLEXION
                            if(w3!=w2){
                                rayPen.setColor(QColor(224, 27, 27));
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
                                            lineIP6toTX->setPen(rayPen);lineIP5toIP6->setPen(rayPen);lineIP4toIP5->setPen(rayPen);lineRXtoIP4->setPen(rayPen);
                                            En = EnCalcultor({lineIP6toTX,lineIP5toIP6,lineIP4toIP5,lineRXtoIP4},lineRXtoMP3);
                                            power += (1/(8*transmitter->Ra))*pow(abs(transmitter->he*En),2);
                                            addItem(lineIP6toTX);addItem(lineIP5toIP6);addItem(lineIP4toIP5);addItem(lineRXtoIP4);

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
        receiverPower->setPlainText(QString::number(receiver->power)+" dBm");
        removeItem(receiverPower);
        addItem(receiverPower);
        receiver->computeDebit();
        receiverDebit->setPlainText(QString::number(receiver->debit)+" Mb/s");
        removeItem(receiverDebit);
        addItem(receiverDebit);
    }

}

bool MapGraphicsScene::isSameSide(Wall* w)
{
    // ATTENTION il faudra penser a prendre en compte le cas ou le RX est sur le mur

    QPointF wallP1 = w->line().p1();
    QPointF wallP2 = w->line().p2();

    bool b1 = (transmitter->x()-wallP1.x())*(wallP2.y()-wallP1.y())-(transmitter->y()-wallP1.y())*(wallP2.x()-wallP1.x())>0;
    bool b2 = (receiver->x()-wallP1.x())*(wallP2.y()-wallP1.y())-(receiver->y()-wallP1.y())*(wallP2.x()-wallP1.x())>0;

    return (b1==b2);
}

qreal MapGraphicsScene::incidenceAngle(QLineF ray,Wall* wall)
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
