#include "mainwindow.h"
#include <iostream>
#include <fstream>
using namespace std;

#include <QFileDialog>
#include <QHeaderView>
#include <QDebug>
#include "signalgraph_model.h"
#include "signalgraph_scene.h"
#include "graphhints_rep.h"
#include "signalgraph_rep.h"
#include "deploymentscene.h"

static const char *const signalgraph_pat = "Signal Float Graphs (*.sml *p1.an *p2.an *.p1 *.p2 *.p6)";
static const char *const graphhints_pat  = "Graphic Hints (*.xml)";
//static const char *const working_dir     = "..\\..\\part-2.0";
//static const char *const working_dir     = "../freeSP-2.0/part-2.0";
static const char *const working_dir     = "../part-2.0";

MainWindow::MainWindow(QApplication &app)
    : _app(app)
    , graph_filename("")
    , hints_filename("")
    , xml_rep(0)
    , graph_model(0)
    , graph_scene(0)
    , deployment_scene(0)
{
    setupUi(this);
    graphicsView->setRenderHints(QPainter::Antialiasing);
    //graphicsView->setViewport(new QGLWidget);
}

/*
 *      About
 */
void MainWindow::on_actionAbout_triggered(){
    cout << "About called" << endl;
}

/*
 *      Open
 */
void MainWindow::on_actionOpen_Graph_triggered(){
    graph_filename = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                 working_dir,
                                                 tr(signalgraph_pat));
    if (!graph_filename.isEmpty()){
        delete xml_rep;
        delete graph_model;
        delete graph_scene;
        delete deployment_scene;
        graph_model      = 0;
        graph_scene      = 0;
        deployment_scene = 0;
        xml_rep     = new XmlFileRep(graph_filename);
        QDomElement root = xml_rep->get_rootElement();
        if (root.nodeName() == QString("signal-graph")){
            graph_model = new SignalgraphModel(this, xml_rep);
            graph_scene = new SignalgraphScene(graph_model);
            graphicsView->setScene(graph_scene);
        } else if (root.nodeName() == QString("deployment")){
            deployment_scene = new DeploymentScene(xml_rep->get_rootElement());
            graphicsView->setScene(deployment_scene);
        }
    }
}

void MainWindow::on_actionOpen_Hints_triggered(){
    hints_filename = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                 working_dir,
                                                 tr(graphhints_pat));
    if (!hints_filename.isEmpty()){
        GraphHintsRep graph_hints(hints_filename);
        if (graph_scene != 0){
            graph_scene->updateGraphHints(&graph_hints);
            graph_scene->update(0, 0, graph_scene->width(), graph_scene->height());
        }
        if (deployment_scene != 0){
            deployment_scene->updateGraphHints(&graph_hints);
            deployment_scene->update(0, 0, deployment_scene->width(), deployment_scene->height());
        }
    }
}

/*
 *      New
 */
#if 0
void MainWindow::on_actionNew_triggered(){
    cout << "New called" << endl;
    delete graph_model;
    delete graph_scene;
    graph_model = 0;
    graph_scene = 0;
    graph_filename = QString("");
    hints_filename = QString("");
}
#endif

/*
 *      Save
 */
void MainWindow::saveHints(){
    qDebug() << "MainWindow::saveHints(" << hints_filename << ")" << endl;
    ofstream fout(hints_filename.toStdString().c_str());
    if (graph_model != 0){
        fout << GraphHintsRep(graph_scene);
    }
    if (deployment_scene != 0){
        fout << GraphHintsRep(deployment_scene);
    }
}

void MainWindow::on_actionSave_Hints_triggered(){
    if (hints_filename.isEmpty()){
        hints_filename = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                 working_dir,
                                                 tr(graphhints_pat));
    }
    if (!hints_filename.isEmpty()){
        saveHints();
    }
}

/*
 *      Save As
 */
void MainWindow::on_actionSave_Hints_As_triggered(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                 working_dir,
                                                 tr(graphhints_pat));
    if (!fileName.isEmpty()){
        hints_filename = fileName;
        saveHints();
    }
}

/*
 *      Exit
 */
void MainWindow::on_actionExit_triggered(){
    delete graph_model; graph_model = 0;
    delete graph_scene; graph_scene = 0;
    _app.quit();
}

