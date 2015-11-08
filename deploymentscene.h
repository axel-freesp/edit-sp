#ifndef DEPLOYMENTSCENE_H
#define DEPLOYMENTSCENE_H

#include <QGraphicsScene>
#include <QPointF>
#include "signalgraph_helper.h"
#include <list>
using namespace std;

class GraphHintsRep;
class ProcessViewItem;

class DeploymentScene: public QGraphicsScene, public SignalgraphHelper {
    Q_OBJECT
public:
    DeploymentScene(QDomElement deployment);
    void updateGraphHints(const GraphHintsRep *new_hints);
    const list<ProcessViewItem*> &getProcessList() const {return process_list;}
private slots:
    void positionChanged(const ProcessViewItem *item);
private:
    list<ProcessViewItem*> process_list;
};

#endif // DEPLOYMENTSCENE_H
