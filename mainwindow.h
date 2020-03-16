#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mapwindow.h"
#include "simwindow.h"
#include "mapgraphicsscene.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
private:
    MapGraphicsScene* scene;
    QTabWidget* tabs;
    int tabsNb =0;
protected:
    void showEvent(QShowEvent* event);
protected slots:
    void addSimTab();
    void closeSimTab(int);
};

#endif // MAINWINDOW_H
