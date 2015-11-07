#ifndef SIGNALGRAPH_SCENE_H
#define SIGNALGRAPH_SCENE_H

#include <QGraphicsScene>
#include <QPointF>
#include "signalgraph_helper.h"
#include <list>
using namespace std;

class SignalgraphModel;
class NodeViewItem;
class EdgeViewItem;
class GraphHintsRep;


class SignalgraphScene: public QGraphicsScene, public SignalgraphHelper {
    Q_OBJECT
public:
    SignalgraphScene(SignalgraphModel *_model);
    void updateGraphHints(const GraphHintsRep *hints);
    const list<NodeViewItem*> &getNodeList() const {return nodeviewitemlist;}
private slots:
    void positionChanged(const NodeViewItem *item);
signals:
    void redrawEdge(const NodeViewItem *item);
private:
    SignalgraphModel *model;
    list<NodeViewItem*> nodeviewitemlist;
    list<EdgeViewItem*> edgeviewitemlist;
};

#endif

