#include "deploymentscene.h"
#include "processviewitem.h"
#include "nodeviewitem.h"
#include "edgeviewitem.h"
#include "graphhints_rep.h"
#include <QDebug>
#include <QGraphicsWidget>
#include <QtXml/QDomElement>

DeploymentScene::DeploymentScene(QDomElement deployment){
    // TODO: Anbindung Model...
    setItemIndexMethod(QGraphicsScene::NoIndex);
    for (QDomElement g=deployment.firstChildElement("signal-graph"); !g.isNull(); g=g.nextSiblingElement("signal-graph")){
        ProcessViewItem *proc = new ProcessViewItem(g, QColor(200,200,200));
        process_list.push_back(proc);
        addItem(proc);
        connect(proc, SIGNAL(newPosition(const ProcessViewItem*)),
                this, SLOT(positionChanged(const ProcessViewItem*)));
    }
    for (list<ProcessViewItem*>::iterator i=process_list.begin(); i!=process_list.end(); ++i){
        for (list<ProcessViewItem*>::iterator j=process_list.begin(); j!=process_list.end(); ++j){
            if ((*i) != (*j)){
                for (list<NodeViewItem*>::iterator k=(*i)->nodeviewitemlist.begin(); k!=(*i)->nodeviewitemlist.end(); ++k){
                    if ((*k)->node.attribute("dest")   == (*j)->graph.attribute("process")){
                        for (list<NodeViewItem*>::iterator l=(*j)->nodeviewitemlist.begin(); l!=(*j)->nodeviewitemlist.end(); ++l){
                            if (   (*l)->node.attribute("source") == (*i)->graph.attribute("process")
                                && (*l)->node.attribute("mode") == (*k)->node.attribute("mode")){
                                ProxyEdgeViewItem *edge = new ProxyEdgeViewItem(*i, *j, *k, *l);
                                addItem(edge);
                                edge->setZValue(3);
                                connect((*i), SIGNAL(positionChanged(const ProcessViewItem*)),
                                        edge, SLOT(redrawEdge(const ProcessViewItem*)));
                                connect((*j), SIGNAL(positionChanged(const ProcessViewItem*)),
                                        edge, SLOT(redrawEdge(const ProcessViewItem*)));
                                connect((*k), SIGNAL(positionChanged(const NodeViewItem*)),
                                        edge, SLOT(redrawEdge(const NodeViewItem*)));
                                connect((*l), SIGNAL(positionChanged(const NodeViewItem*)),
                                        edge, SLOT(redrawEdge(const NodeViewItem*)));
                            }
                        }
                    }
                }
            }
        }
    }
}

void DeploymentScene::updateGraphHints(const GraphHintsRep *new_hints){
    for (list<ProcessViewItem*>::iterator i=process_list.begin(); i!=process_list.end(); ++i){
        QRect r = new_hints->getProcessHintRect((*i)->graph.attribute("process"), "deployment");
        //qDebug() << "DeploymentScene::updateGraphHints: " << (*i)->graph.attribute("process") << ":" << r;
        if ((r.width() > 0) && (r.height() > 0)){
            QString processname = (*i)->graph.attribute("process");
            (*i)->setNewRect(r);
            for (list<NodeViewItem*>::iterator j=(*i)->nodeviewitemlist.begin(); j!=(*i)->nodeviewitemlist.end(); ++j){
                QString nodename   = (*j)->node.attribute("name");
                QString iotypename = (*j)->node.attribute("io-type");
                if (nodename.startsWith("input_proxy_")){
                    QString sourcename = (*j)->node.attribute("source");
                    QPoint p = new_hints->getInProxyHintPos(processname, sourcename, iotypename, "deployment");
                    (*j)->setNewPosition(p);
                } else if (nodename.startsWith("output_proxy_")){
                    QString destname = (*j)->node.attribute("dest");
                    QPoint p = new_hints->getOutProxyHintPos(processname, destname, iotypename, "deployment");
                    (*j)->setNewPosition(p);
                } else if (!nodename.isNull()){
                    QPoint p = new_hints->getNodeHintPos(nodename, "deployment");
                    (*j)->setNewPosition(p);
                } else {
                    ;
                }
            }
        }
    }
    setSceneRect(itemsBoundingRect());
}

void DeploymentScene::positionChanged(const ProcessViewItem *item){
    //qDebug() << "DeploymentScene::positionChanged";
    setSceneRect(itemsBoundingRect());
}

