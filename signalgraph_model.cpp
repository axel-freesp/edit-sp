#include "signalgraph_model.h"
#include <string.h>
#include <QPointF>

SignalgraphModel::SignalgraphModel(QObject *parent, XmlFileRep *xml_file)
    : QAbstractItemModel(parent)
    , signal_graph(xml_file)
{
    QDomElement root = signal_graph.get_rootElement();
    if (!root.isNull()){
        QDomElement nodes = root.firstChildElement("nodes");
        nodesnode.type    = nodelistT;
        nodesnode.elem1   = nodes;
        if (!nodes.isNull()){
            for (QDomElement e=nodes.firstChildElement(); !e.isNull(); e=e.nextSiblingElement()){
                ModelNode *n = new ModelNode(nodeT, e);
                nodelist.push_back(n);
            }
        }
        QDomElement edges = root.firstChildElement("connections");
        edgesnode.type    = edgelistT;
        edgesnode.elem1   = edges;
        if (!edges.isNull()){
            for (QDomElement e=edges.firstChildElement(); !e.isNull(); e=e.nextSiblingElement()){
                QDomElement src, dst;
                ModelNodeList::iterator n;
                const QString from(e.attribute("from")), to(e.attribute("to"));
                for (n=nodelist.begin(); n!=nodelist.end(); ++n){
                    if ((*n)->elem1.attribute("name") == from){
                        src = (*n)->elem1;
                    }
                    if ((*n)->elem1.attribute("name") == to){
                        dst = (*n)->elem1;
                    }
                }
                ModelNode *m = new ModelNode(edgeT, e, src, dst);
                edgelist.push_back(m);
            }
        }
    }
}

SignalgraphModel::~SignalgraphModel(){}


QModelIndex SignalgraphModel::index(int row, int column, const QModelIndex &parent) const {
    QModelIndex ret;
    if (!parent.isValid()){ // toplevel
        if (column == 0){
            switch (row){
            case 0:
                ret = createIndex(row, column, (void*)&nodesnode);
            case 1:
                ret = createIndex(row, column, (void*)&signaltypesnode);
            case 2:
                ret = createIndex(row, column, (void*)&edgesnode);
            default:
                ;
            }
        }
    } else {
        const ModelNode *p = (const ModelNode*)parent.internalPointer();
        ModelNodeList::const_iterator i;
        int r = 0;
        switch (p->type){
        case nodelistT:
            for (i=nodelist.begin(); i!=nodelist.end(); ++i){
                if (r++ == row){
                    break;
                }
            }
            if (i != nodelist.end()){
                ret = createIndex(row, column, (void*)(*i));
            }
            break;
        case signaltypelistT:
            for (i=signaltypelist.begin(); i!=signaltypelist.end(); ++i){
                if (r++ == row){
                    break;
                }
            }
            if (i != signaltypelist.end()){
                ret = createIndex(row, column, (void*)(*i));
            }
            break;
        case edgelistT:
            for (i=edgelist.begin(); i!=edgelist.end(); ++i){
                if (r++ == row){
                    break;
                }
            }
            if (i != edgelist.end()){
                ret = createIndex(row, column, (void*)(*i));
            }
            break;
        default:
            ;
        }
    }
    return ret;
}

QModelIndex SignalgraphModel::parent(const QModelIndex &index) const {
    if (index.isValid()){
        const ModelNode *n = (const ModelNode*)index.internalPointer();
        switch(n->type){
        case nodeT:
            return createIndex(0, 0, (void*)&nodesnode);
        case signaltypeT:
            return createIndex(1, 0, (void*)&signaltypesnode);
        case edgeT:
            return createIndex(2, 0, (void*)&edgesnode);
        default:
            ;
        }
    }
    return QModelIndex();
}

int SignalgraphModel::rowCount(const QModelIndex &parent) const {
    if (!parent.isValid()){
        return 3;
    }
    const ModelNode *p = (const ModelNode*)parent.internalPointer();
    switch (p->type){
    case nodelistT:
        return nodelist.size();
    case signaltypelistT:
        return signaltypelist.size();
    case edgelistT:
        return edgelist.size();
    default:
        return 0;
    }
}

int SignalgraphModel::columnCount(const QModelIndex &/*parent*/) const {
    return 1;
}

QVariant SignalgraphModel::data(const QModelIndex &index, int role) const {
    ModelNode *n = (ModelNode*)index.internalPointer();
    if (n){
        switch (role){
        case Qt::DisplayRole:
            switch (n->type){
            case nodelistT:
                return QVariant(QString("nodes"));
            case signaltypelistT:
                return QVariant(QString("signal-types"));
            case edgelistT:
                return QVariant(QString("edges"));
            case nodeT: case signaltypeT:
                return QVariant(n->elem1.attribute("name"));
            case edgeT:
                return QVariant(n->elem1.attribute("from") + QString("->") + n->elem1.attribute("to"));
            default:
                return QVariant(QString("(invalid)"));
            }
        case GraphRole:
            return qVariantFromValue<void*>((void*)n);
        default:
            ;
        }
    }
    return QVariant();
}

bool SignalgraphModel::hasChildren(const QModelIndex &parent) const {
    if (!parent.isValid()){
        return true;
    }
    const ModelNode *p = (const ModelNode*)parent.internalPointer();
    switch (p->type){
    case nodelistT: case signaltypelistT: case edgelistT:
        return true;
    default:
        return false;
    }
}

QModelIndex SignalgraphModel::index(ModelNodeType type) const {
    switch (type){
    case nodelistT:
        return createIndex(0, 0, (void*)&nodesnode);
    case signaltypelistT:
        return createIndex(0, 0, (void*)&signaltypesnode);
    case edgelistT:
        return createIndex(0, 0, (void*)&edgesnode);
    default:
        return QModelIndex();
    }
}

void SignalgraphModel::updateNodePosition(const QModelIndex & index, const QPointF &new_position){
    ModelNode *n = (ModelNode*)index.internalPointer();
    if (n->type == nodeT){
//        SetNodeToplevelPosition(n->elem1, new_position);
    }
}

