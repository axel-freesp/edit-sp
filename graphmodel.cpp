#include "graphmodel.h"
#include "nodeviewitem.h"
#include <iostream>
#include <string.h>
using namespace std;

GraphModel::GraphModel(QObject *parent, const QString &graph_filename)
    : QAbstractItemModel(parent)
    , signal_graph(graph_filename.toStdString().c_str())
    , components(0)
    , nodes(0)
    , nodetypes(0)
{
    DOMElement *root = signal_graph.dom_document->getDocumentElement();
    if (root){
        nodes = FindChildByName(root, "nodes");
        const QString components_filename(ComponentsFilename());
        if (!components_filename.isEmpty()){
            try {
                const QString slash("/");
                QString p(graph_filename);
                int last = graph_filename.lastIndexOf(slash);
                p.replace(last, p.length() - last, QString("/../../libraries"));
                cout << "Search components at " << p.toStdString() << endl;
                cout << "Open components " << components_filename.toStdString() << endl;
                components = new ComponentsRep(components_filename.toStdString().c_str(),
                                               p.toStdString().c_str());

                nodetypes = components->dom_document->getDocumentElement();
            } catch (const char *error) {
                cerr << error << endl;
                components = 0;
            }
        }
        DOMElement *e;
        for (e=FirstChild(nodes); e; e=NextSibling(e)){
            DOMElement *t(0);
            if (!strcmp(NAME(e), "processing-node")){
                const XML_Char type = ATTR(e, "type");
                for (t=FirstChild(nodetypes); t; t=NextSibling(t)){
                    if (!strcmp(NAME(t), "node-type") && !strcmp(ATTR(t, "name"), type)){
                        break;
                    }
                }
            }
            NodeViewItem *i = new NodeViewItem(e, t);
            addItem(i);
        }
    }
}

GraphModel::~GraphModel(){}


QModelIndex GraphModel::index(int row, int column, const QModelIndex &parent) const {
    int r = 0;
    if (!parent.isValid()){ // toplevel
        if (column == 0){
            switch (row){
            case 0:
                return createIndex(row, column, (void*)nodes);
            case 1:
                return createIndex(row, column, (void*)nodetypes);
            default:
                ;
            }
        }
    } else {
        const DOMElement *p = (const DOMElement *)parent.internalPointer();
        if (nodes && (p == nodes)){
            const DOMElement *e;
            for (e=FirstChild(nodes); e; e=NextSibling(e)){
                if (r++ == row){
                    break;
                }
            }
            switch (column){
            case 0:
                if (e){
                    return createIndex(row, column, (void*)e);
                }
                break;
            case 1:
                if (e && !strcmp(NAME(e), "processing-node")){
                    const QString type(ATTR(e, "type"));
                    for (e=FirstChild(nodetypes); e; e=NextSibling(e)){
                        if (!strcmp(NAME(e), "node-type")){
                            if (!type.compare(QString(ATTR(e, "name")))){
                                return createIndex(row, column, (void*)e);
                            }
                        }
                    }
                }
                break;
            default:
                ;
            }
        }
        if (nodetypes && (p == nodetypes)){
            const DOMElement *e;
            switch (column){
            case 0:
                for (e=FirstChild(nodetypes); e; e=NextSibling(e)){
                    if (!strcmp(NAME(e), "node-type")){
                        if (r++ == row){
                            return createIndex(row, column, (void*)e);
                        }
                    }
                }
                break;
            default:
                ;
            }
        }
    }
    return QModelIndex();
}

QModelIndex GraphModel::parent(const QModelIndex &index) const {
    if (index.isValid()){
        const DOMElement *e = (const DOMElement *)index.internalPointer();
        if ((e == nodetypes) || (e == nodes)){
            return QModelIndex();
        }
        const DOMElement *p = (const DOMElement*)e->getParentNode();
        if (p == nodetypes){
            return createIndex(0, 0, (void*)nodetypes);
        } else if (p == nodes){
            return createIndex(0, 0, (void*)nodes);
        }
    }
    return QModelIndex();
}

int GraphModel::rowCount(const QModelIndex &parent) const {
    if (!parent.isValid()){
        return 2;
    }
    const DOMElement *e = (const DOMElement *)parent.internalPointer();
    if (e == nodes){
        return signal_graph.NodesCount();
    }
    if (e == nodetypes){
        return components->NodetypesCount();
    }
    return 0;
}

int GraphModel::columnCount(const QModelIndex &parent = QModelIndex()) const {
    return 2;
}

QVariant GraphModel::data(const QModelIndex &index, int role) const {
    const DOMElement *e = (const DOMElement *)index.internalPointer();
    if (e){
        switch (role){
        case Qt::DisplayRole:
            if (e == nodes){
                return QVariant(QString("nodes"));
            }
            if (e == nodetypes){
                return QVariant(QString("node-types"));
            }
            return QVariant(QString(ATTR(e, "name")));
        case Qt::SizeHintRole:
            //cout << "Qt::SizeHintRole " << (e == nodes) << endl;
        default:
            ;
        }
    }
    return QVariant();
}

bool GraphModel::hasChildren(const QModelIndex &parent) const {
    if (!parent.isValid()){
        return true;
    }
    const DOMElement *e = (const DOMElement *)parent.internalPointer();
    return (e == nodetypes) || (e == nodes);
}

QString GraphModel::ComponentsFilename() const {
    const DOMElement *root = signal_graph.dom_document->getDocumentElement();
    if (root){
        const DOMElement *library = FindChildByName(root, "library");
        if (library){
            return QString(ATTR(library, "ref"));
        }
    }
    return QString();
}


