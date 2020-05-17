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
        void updateBar();
private:
        void addMenuBar();
        std::vector<int> iterListI;
        std::vector<int> iterListJ;
        QMenuBar* menuBar;
        int iter=0;
        QToolBar* toolBar;
        QProgressBar* loadBar;
        int transparency;
        QTimer* timer;
        QAction* loadAction;
        QAction* dBmBtn;
        QAction* noCoBtn;
        QSlider* blurSlider;
        SimGraphicsScene* scene;
        QGraphicsView* view;
        void save();
        QRectF removeMargin(const QRectF& sceneRect, const QSize& viewerSize);
};

#endif // SIMWINDOW_H
