#include "graphhints_rep.h"
#include "signalgraph_scene.h"
#include "deploymentscene.h"
#include "nodeviewitem.h"
#include "graphhint_node.h"
#include "processviewitem.h"
#include <list>
#include <QDebug>
using namespace std;

static const int g_border(10);
static const int g_extraFrame(400);

typedef list<GH_Node*> GH_List;
typedef list<ProcessViewItem*> PVI_List;

static QRect getBoundingRect(const GH_List &list){
    GH_List::const_iterator n = list.begin();
    if (n != list.end()){
        QRect rect = (*n)->box;
        for (++n; n!=list.end(); ++n){
            rect = rect.united((*n)->box);
        }
        rect.setTopLeft(rect.topLeft() - QPoint(g_border, g_border));
        rect.setBottomRight(rect.topLeft() + QPoint(rect.width() + 2 * g_border, rect.height() + 2 * g_border));
        return rect;
    }
    return QRect();
}

static QRect getBoundingRect(const list<ProcessViewItem*> &list){
    PVI_List::const_iterator n = list.begin();
    if (n != list.end()){
        QRect rect((*n)->left(), (*n)->top(), (*n)->width(), (*n)->height());
        for (++n; n!=list.end(); ++n){
            QRect r((*n)->left(), (*n)->top(), (*n)->width(), (*n)->height());
            rect = rect.united(r);
        }
        rect.setTopLeft(rect.topLeft() - QPoint(g_border, g_border));
        rect.setBottomRight(rect.topLeft() + QPoint(rect.width() + 2 * g_border, rect.height() + 2 * g_border));
        return rect;
    }
    return QRect();
}

GraphHintsRep::GraphHintsRep(const QString &filename)
    : XmlFileRep(filename)
{
    QDomElement root = get_rootElement();
    if (!root.isNull()){
        for (QDomElement n=root.firstChildElement("graph-hint"); !n.isNull(); n=n.nextSiblingElement("graph-hint")){
            const QString name    = n.attribute("name");
            const QString process = n.attribute("process");
            const QString iotype  = n.attribute("io-type");
            const QString source  = n.attribute("source");
            const QString dest    = n.attribute("dest");
            const QString w       = n.attribute("w");
            if (!name.isNull()){
                node_list.push_back(new GH_PNodeNode(n));
            } else if (!source.isNull()){
                node_list.push_back(new GH_InProxyNode(n));
            } else if (!dest.isNull()){
                node_list.push_back(new GH_OutProxyNode(n));
            } else if (!process.isNull()){
                node_list.push_back(new GH_ProcessNode(n));
            } else {
                node_list.push_back(new GH_FrameNode(n));
            }
        }
    }
}

GraphHintsRep::GraphHintsRep(const SignalgraphScene *scene){
    QDomElement root = domDocument.createElementNS("http://www.freesp.de/xml/freeSP", "graph-hints");
    domDocument.appendChild(root);
    for (list<NodeViewItem*>::const_iterator j=scene->getNodeList().begin(); j!=scene->getNodeList().end(); ++j){
        QDomElement e = domDocument.createElement("graph-hint");
        QString name = (*j)->node.attribute("name");
        QString process = (*j)->node.attribute("process");
        if (name.startsWith("input_proxy_")){
            node_list.push_back(new GH_InProxyNode(e, *j));
        } else if (name.startsWith("output_proxy_")){
            node_list.push_back(new GH_OutProxyNode(e, *j));
        } else if (!name.isNull()){
            node_list.push_back(new GH_PNodeNode(e, *j));
        }
        root.appendChild(e);
    }
}

GraphHintsRep::GraphHintsRep(const DeploymentScene *scene){
    QDomElement root = domDocument.createElementNS("http://www.freesp.de/xml/freeSP", "graph-hints");
    domDocument.appendChild(root);
    const list<ProcessViewItem*> &prlist = scene->getProcessList();
    QRect bbox = getBoundingRect(prlist);
    QDomElement e = domDocument.createElement("graph-hint");
    node_list.push_back(new GH_FrameNode(e, bbox, "deployment"));
    root.appendChild(e);
    //qDebug() << "GraphHintsRep::GraphHintsRep bbox=" << bbox;
    for (list<ProcessViewItem*>::const_iterator i=prlist.begin(); i!=prlist.end(); ++i){
        QDomElement e = domDocument.createElement("graph-hint");
        node_list.push_back(new GH_ProcessNode(e, *i, "deployment", bbox));
        root.appendChild(e);
        //qDebug() << "GraphHintsRep::GraphHintsRep process" << (*(--(node_list.end())))->getProcess() << (*(--(node_list.end())))->box;
        for (list<NodeViewItem*>::const_iterator j=(*i)->nodeviewitemlist.begin(); j!=(*i)->nodeviewitemlist.end(); ++j){
            QDomElement e = domDocument.createElement("graph-hint");
            QString name = (*j)->node.attribute("name");
            QString process = (*j)->node.attribute("process");
            if (name.startsWith("input_proxy_")){
                node_list.push_back(new GH_InProxyNode(e, *j, "deployment", *i));
            } else if (name.startsWith("output_proxy_")){
                node_list.push_back(new GH_OutProxyNode(e, *j, "deployment", *i));
            } else if (!name.isNull()){
                node_list.push_back(new GH_PNodeNode(e, *j, "deployment", *i));
            }
            root.appendChild(e);
        }
    }
}

GraphHintsRep::~GraphHintsRep(){}

QPoint GraphHintsRep::getNodeHintPos(const QString &nodename, const QString &mode) const {
    for (GH_List::const_iterator n=node_list.begin(); n!=node_list.end(); ++n){
        if (   ((*n)->getName() == nodename)
            && ((*n)->getMode() == mode)
        ){
            return (*n)->pos();
        }
    }
    return QPoint();
}

QPoint GraphHintsRep::getInProxyHintPos(const QString &processname, const QString &sourcename, const QString &iotypename, const QString &mode) const {
    for (GH_List::const_iterator n=node_list.begin(); n!=node_list.end(); ++n){
        if (   ((*n)->getProcess() == processname)
            && ((*n)->getSource()  == sourcename)
            && ((*n)->getIOType()  == iotypename)
            && ((*n)->getMode()    == mode)
        ){
            return (*n)->pos();
        }
    }
    return QPoint();
}

QPoint GraphHintsRep::getOutProxyHintPos(const QString &processname, const QString &destname, const QString &iotypename, const QString &mode) const {
    for (GH_List::const_iterator n=node_list.begin(); n!=node_list.end(); ++n){
        if (   ((*n)->getProcess() == processname)
            && ((*n)->getDest()    == destname)
            && ((*n)->getIOType()  == iotypename)
            && ((*n)->getMode()    == mode)
        ){
            return (*n)->pos();
        }
    }
    return QPoint();
}

QRect GraphHintsRep::getProcessHintRect(const QString &processname, const QString &mode) const{
    const GH_Node *n = getProcessHint(processname, mode);
    if (n != 0){
        return n->box;
    }
    return QRect();
}

GH_Node       *GraphHintsRep::getProcessHint(const QString &processname, const QString &mode){
    for (GH_List::iterator n=node_list.begin(); n!=node_list.end(); ++n){
        if (   ((*n)->getProcess() == processname)
            && ((*n)->getMode()    == mode)
        ){
            return (*n);
        }
    }
    return 0;
}

const GH_Node *GraphHintsRep::getProcessHint(const QString &processname, const QString &mode) const{
    for (GH_List::const_iterator n=node_list.begin(); n!=node_list.end(); ++n){
        if (   ((*n)->getProcess() == processname)
            && ((*n)->getMode()    == mode)
        ){
            return (*n);
        }
    }
    return 0;
}

