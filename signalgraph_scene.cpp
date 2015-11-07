#include "signalgraph_scene.h"
#include "signalgraph_model.h"
#include "nodeviewitem.h"
#include "edgeviewitem.h"
#include "graphhints_rep.h"
#include <QModelIndex>
#include <iostream>
#include <list>
using namespace std;

typedef SignalgraphModel::ModelNode ModelNode;

SignalgraphScene::SignalgraphScene(SignalgraphModel *_model)
    : model(_model)
{
    setItemIndexMethod(QGraphicsScene::NoIndex);
    QModelIndex index = model->index(SignalgraphModel::nodelistT);
    if (index.isValid()){
        //cout << "SignalgraphScene: valid nodelist" << endl;
        QModelIndex parent(index);
        int n_nodes = model->rowCount(parent);
        for (int i=0; i<n_nodes; ++i){
            index = model->index(i, 0, parent);
            if (index.isValid()){
                const ModelNode *node = (const ModelNode*)index.data(SignalgraphModel::GraphRole).value<void*>();
                NodeViewItem *item = new NodeViewItem(node->elem1);
                addItem(item);
                nodeviewitemlist.push_back(item);
                connect(item, SIGNAL(positionChanged(const NodeViewItem*)),
                        this, SLOT(positionChanged(const NodeViewItem*)));
            }
        }
    }
    index = model->index(SignalgraphModel::edgelistT);
    if (index.isValid()){
        //cout << "SignalgraphScene: valid edgelist" << endl;
        QModelIndex parent(index);
        int         n_edges = model->rowCount(parent);
        //cout << "SignalgraphScene: " << n_edges << " edges" << endl;
        for (int i=0; i<n_edges; ++i){
            index = model->index(i, 0, parent);
            if (index.isValid()){
                const ModelNode *edge = (const ModelNode*)index.data(SignalgraphModel::GraphRole).value<void*>();
                int from_index = GetOutportIndex(edge->elem2, edge->elem1.attribute("from-port"));
                int to_index   = GetInportIndex(edge->elem3, edge->elem1.attribute("to-port"));
                const NodeViewItem *from = 0, *to = 0;
                for (list<NodeViewItem*>::const_iterator j=nodeviewitemlist.begin(); j!=nodeviewitemlist.end(); ++j){
                    if ((*j)->node.attribute("name") == edge->elem1.attribute("from")){
                        from = *j;
                    }
                    if ((*j)->node.attribute("name") == edge->elem1.attribute("to")){
                        to = *j;
                    }
                }
                if ((from != 0) && (to != 0)){
                    EdgeViewItem *item = new EdgeViewItem(from, to, from_index, to_index);
                    item->setZValue(1);
                    addItem(item);
                    edgeviewitemlist.push_back(item);
                    list<NodeViewItem*>::iterator nvi;
                    for (nvi=nodeviewitemlist.begin(); nvi!=nodeviewitemlist.end(); ++nvi){
                        if (((*nvi)->node == edge->elem2) || ((*nvi)->node == edge->elem3)){
                            connect((*nvi), SIGNAL(positionChanged(const NodeViewItem*)),
                                    item, SLOT(redrawEdge(const NodeViewItem*)));
                        }
                    }
                }
            }
        }
    }
}

void SignalgraphScene::positionChanged(const NodeViewItem *item){
    QModelIndex parent   = model->index(SignalgraphModel::nodelistT);
    QString     itemname = item->node.attribute("name");
    int         n_nodes  = model->rowCount(parent);
    for (int i=0; i<n_nodes; ++i){
        QModelIndex index = model->index(i, 0, parent);
        if (index.isValid()){
            const ModelNode *node = (const ModelNode*)index.data(SignalgraphModel::GraphRole).value<void*>();
            if (node->elem1.attribute("name") == itemname){
                model->updateNodePosition(index, item->pos());
                break;
            }
        }
    }
    setSceneRect(itemsBoundingRect());
}

void SignalgraphScene::updateGraphHints(const GraphHintsRep *hints){
    list<NodeViewItem*>::iterator nvi;
    for (nvi=nodeviewitemlist.begin(); nvi!=nodeviewitemlist.end(); ++nvi){
        QPoint p = hints->getNodeHintPos((*nvi)->node.attribute("name"));
        (*nvi)->setNewPosition(p);
    }
    setSceneRect(itemsBoundingRect());
}


