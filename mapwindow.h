#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <QMainWindow>
#include "mapgraphicsscene.h"
#include <QGraphicsView>
#include <QToolBar>
#include <QComboBox>
#include <QMenuBar>
#include <QLabel>


class MapWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MapWindow(MapGraphicsScene* scene, QGraphicsView* view,QWidget *parent = nullptr);
        ~MapWindow();
        QAction* runAction;
    private:
        QGraphicsView* view;
        MapGraphicsScene* scene;
    // ToolBar
        QMenuBar* menuBar;
        QToolBar* toolBar;
        QToolBar* labelBar;
    // Actions
        QAction* receiverBtn;
        QAction* transmitterBtn;
        QAction* undo;
        QAction* redo;
        QAction* clear;
        QAction* hidePointsAction;
        QAction* hideRaysAction;
        QAction* grayAction;
        QAction* whiteAction;
        QToolButton* runBtn;
    // ComboBox
        QComboBox* resolutionBox;
        QComboBox* materialBox;
    // Labels
        QLabel *wallThickness;
        QLabel* positionLabel;
        QLabel* lengthLabel;
        QLabel* distanceLabel;



        void addActionsToToolbar();
        void addLabelsToLabelbar();
        void addActionsToMenubar();
protected:
        void changeThickness(int);
        void resizeEvent(QResizeEvent* event);
        void showEvent(QShowEvent* event);
        void changeColorToLightgray();
        void changeColorToWhite();

};

#endif // MAPWINDOW_H
