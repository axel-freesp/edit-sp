#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QFile>

#include "ui_frontend.h"
class SignalgraphModel;
class SignalgraphScene;
class DeploymentScene;
class GraphHintsRep;
class XmlFileRep;

class MainWindow : public QMainWindow
                 , private Ui_MainWindow
{
     Q_OBJECT

public:
     MainWindow(QApplication &app);
private slots:
     void on_actionAbout_triggered();
     void on_actionOpen_Graph_triggered();
     void on_actionOpen_Hints_triggered();
     //void on_actionNew_triggered();
     void on_actionSave_Hints_triggered();
     void on_actionSave_Hints_As_triggered();
     void on_actionExit_triggered();
private:
    void saveHints();
private:
    QApplication &_app;
    QString graph_filename, hints_filename;
    XmlFileRep       *xml_rep;
    SignalgraphModel *graph_model;
    SignalgraphScene *graph_scene;
    DeploymentScene  *deployment_scene;
};

#endif

