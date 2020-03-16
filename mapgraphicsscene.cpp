#include "mapgraphicsscene.h"

MapGraphicsScene::MapGraphicsScene(){
    this->size=1000;
    this->ratio=2;
    setSceneRect(0,0,ratio*size,size);
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
    float frameNb = 20/resolution.toFloat();
    for(int i =0;i<=frameNb;i++){
       QGraphicsLineItem* line = new QGraphicsLineItem(0,i*size/frameNb,ratio*size,i*size/frameNb);
       line->setPen(gridPen);
       gridLines.push_back(line);
       tempList.push_back(line);
       addItem(line);
    }
    for(int i =0;i<=ratio*frameNb;i++){
       QGraphicsLineItem* line = new QGraphicsLineItem(i*size/frameNb,0,i*size/frameNb,size);
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
                            emit sendLength("Length : "+QString::number(2000*wall->line().length()/size)+" cm ");
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
        emit sendLength("Length : "+QString::number(2000*tempLine->line().length()/size)+" cm ");

    }
    if(transmitterActivated){
        transmitter->setPos(event->scenePos().x(),event->scenePos().y());
        if(receiver){
            drawRXTX();
        }
    }
    if(receiverActivated){
        receiver->setPos(event->scenePos().x(),event->scenePos().y());
        if(transmitter){
            drawRXTX();
        }
    }
    if(receiver&&transmitter){
        sendDistance("TX/RX Distance : "+QString::number(2000*ray1->line().length()/size)+" cm");
    }
    sendPosition("x : "+QString::number(2000*event->scenePos().x()/size)+" cm y : "+QString::number(2000*event->scenePos().y()/size)+" cm ");
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
        pointsAreHidden=true;
        addItem(receiver);
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
    if(receiver){ removeItem(receiver);receiver=nullptr;}
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
    QString fileName= QFileDialog::getSaveFileName(nullptr, tr("Open File"), QCoreApplication::applicationDirPath()+"/saves/maps/MAP"+date, "Ray File (*.ray)" );
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
                                                        tr("Open File"),QCoreApplication::applicationDirPath()+"/saves/maps",
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
            }
        file.close();
        draw(resolution);
        }
    }

}

void MapGraphicsScene::drawRays()
{
    if(!raysAreHidden){
        foreach(QGraphicsLineItem* ray,rayList) removeItem(ray);
        rayList.clear();
        QPen rayPen(QColor(106, 224, 27));
        ray1= new QGraphicsLineItem(transmitter->x(),transmitter->y(),receiver->x(),receiver->y());
        ray1->setPen(rayPen);
        rayList.push_back(ray1);
        addItem(ray1);
        for(Wall* w1:wallList){
            rayPen.setColor(QColor(224, 221, 27));
            if(isSameSide(w1)){
                QLineF wline = w1->line();
                float a = (wline.p2().y()-wline.p1().y())/wline.length();
                float b = (wline.p1().x()-wline.p2().x())/wline.length();
                float c = -a*wline.p1().x()-b*wline.p1().y();
                float dist = a*transmitter->x()+b*transmitter->y()+c;
                QPointF mirrorPoint(transmitter->x()-2*a*dist,transmitter->y()-2*b*dist);
                QPointF intersectPoint;
                QLineF lineRXtoMP1(receiver->x(),receiver->y(),mirrorPoint.x(),mirrorPoint.y());
                if(wline.intersects(lineRXtoMP1,&intersectPoint)==QLineF::BoundedIntersection){
                    QGraphicsLineItem* ray2 = new QGraphicsLineItem(transmitter->x(),transmitter->y(),intersectPoint.x(),intersectPoint.y());
                    QGraphicsLineItem* ray3 = new QGraphicsLineItem(intersectPoint.x(),intersectPoint.y(),receiver->x(),receiver->y());
                    rayList.push_back(ray2);
                    rayList.push_back(ray3);
                    ray2->setPen(rayPen);
                    ray3->setPen(rayPen);
                    addItem(ray2);addItem(ray3);
                    }
                for(Wall* w2:wallList){
                    rayPen.setColor(QColor(224, 152, 27));
                    if(w2!=w1){
                        QLineF wline2 = w2->line();
                        float a2 = (wline2.p2().y()-wline2.p1().y())/wline2.length();
                        float b2 = (wline2.p1().x()-wline2.p2().x())/wline2.length();
                        float c2 = -a2*wline2.p1().x()-b2*wline2.p1().y();
                        float dist2 = a2*mirrorPoint.x()+b2*mirrorPoint.y()+c2;
                        QPointF mirrorPoint2(mirrorPoint.x()-2*a2*dist2,mirrorPoint.y()-2*b2*dist2);

                        QPointF intersectPoint2;
                        QLineF lineRXtoMP2(receiver->x(),receiver->y(),mirrorPoint2.x(),mirrorPoint2.y());
                        if(wline2.intersects(lineRXtoMP2,&intersectPoint2)==QLineF::BoundedIntersection){
                            QLineF lineIP2toMP1(intersectPoint2.x(),intersectPoint2.y(),mirrorPoint.x(),mirrorPoint.y());
                            QPointF intersectPoint3;
                            if(wline.intersects(lineIP2toMP1,&intersectPoint3)==QLineF::BoundedIntersection){
                                QGraphicsLineItem* lineIP3toTX= new QGraphicsLineItem(intersectPoint3.x(),intersectPoint3.y(),transmitter->x(),transmitter->y());
                                QGraphicsLineItem* lineRXtoIP2 = new QGraphicsLineItem(receiver->x(),receiver->y(),intersectPoint2.x(),intersectPoint2.y());
                                QGraphicsLineItem* lineIP2toIP3 = new QGraphicsLineItem(intersectPoint2.x(),intersectPoint2.y(),intersectPoint3.x(),intersectPoint3.y());
                                rayList.push_back(lineIP3toTX);rayList.push_back(lineRXtoIP2);rayList.push_back(lineIP2toIP3);
                                lineIP3toTX->setPen(rayPen);lineRXtoIP2->setPen(rayPen);lineIP2toIP3->setPen(rayPen);
                                addItem(lineIP3toTX);addItem(lineRXtoIP2);addItem(lineIP2toIP3);
                            }

                        }

                        for(Wall* w3:wallList){

                            rayPen.setColor(QColor(224, 27, 27));
                            if(w3!=w2){
                                QLineF wline3 = w3->line();
                                float a3 = (wline3.p2().y()-wline3.p1().y())/wline3.length();
                                float b3 = (wline3.p1().x()-wline3.p2().x())/wline3.length();
                                float c3 = -a3*wline3.p1().x()-b3*wline3.p1().y();
                                float dist3 = a3*mirrorPoint2.x()+b3*mirrorPoint2.y()+c3;

                                QPointF mirrorPoint3(mirrorPoint2.x()-2*a3*dist3,mirrorPoint2.y()-2*b3*dist3);
                                QPointF intersectPoint4;
                                QLineF lineRXtoMP3(receiver->x(),receiver->y(),mirrorPoint3.x(),mirrorPoint3.y());
                                if(wline3.intersects(lineRXtoMP3,&intersectPoint4)==QLineF::BoundedIntersection){
                                    QLineF lineIP4toMP2(intersectPoint4.x(),intersectPoint4.y(),mirrorPoint2.x(),mirrorPoint2.y());
                                    QPointF intersectPoint5;
                                    if(wline2.intersects(lineIP4toMP2,&intersectPoint5)==QLineF::BoundedIntersection){
                                        QLineF lineIP5toMP1(intersectPoint5.x(),intersectPoint5.y(),mirrorPoint.x(),mirrorPoint.y());
                                        QPointF intersectPoint6;
                                        if(wline.intersects(lineIP5toMP1,&intersectPoint6)==QLineF::BoundedIntersection){
                                            QGraphicsLineItem* lineIP6toTX = new QGraphicsLineItem(intersectPoint6.x(),intersectPoint6.y(),transmitter->x(),transmitter->y());
                                            QGraphicsLineItem* lineIP5toIP6 = new QGraphicsLineItem(intersectPoint5.x(),intersectPoint5.y(),intersectPoint6.x(),intersectPoint6.y());
                                            QGraphicsLineItem* lineIP4toIP5 = new QGraphicsLineItem(intersectPoint4.x(),intersectPoint4.y(),intersectPoint5.x(),intersectPoint5.y());
                                            QGraphicsLineItem* lineRXtoIP4 = new QGraphicsLineItem(receiver->x(),receiver->y(),intersectPoint4.x(),intersectPoint4.y());
                                            rayList.push_back(lineIP6toTX);rayList.push_back(lineIP5toIP6);rayList.push_back(lineIP4toIP5);rayList.push_back(lineRXtoIP4);
                                            lineIP6toTX->setPen(rayPen);lineIP5toIP6->setPen(rayPen);lineIP4toIP5->setPen(rayPen);lineRXtoIP4->setPen(rayPen);
                                            addItem(lineIP6toTX);addItem(lineIP5toIP6);addItem(lineIP4toIP5);addItem(lineRXtoIP4);

                                        }
                                    }

                                }
                            }

                        }

                    }
                }
            }

        }
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
