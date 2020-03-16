#ifndef SIMWINDOW_H
#define SIMWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QGraphicsView>
#include "simgraphicsscene.h"
#include "mapgraphicsscene.h"
#include <QComboBox>
#include <QLabel>

class SimWindow : public QMainWindow
{
    Q_OBJECT

    public:
        SimWindow(MapGraphicsScene* scene, QWidget *parent=nullptr);
        ~SimWindow();

protected:
        void resizeEvent(QResizeEvent *event);
        void showEvent(QShowEvent *event);
protected slots:

private:
        void addMenuBar();
        QMenuBar* menuBar;
        QToolBar* toolBar;
        int transparency;
        QLabel* transparencyLabel;
        SimGraphicsScene* scene;
        QGraphicsView* view;
        void save();

};

#endif // SIMWINDOW_H
