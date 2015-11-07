#ifndef SIGNALGRAPH_MODEL_H
#define SIGNALGRAPH_MODEL_H

#include <QAbstractItemModel>
#include "signalgraph_rep.h"
#include <iostream>
#include <list>
using namespace std;

class SignalgraphModel: public QAbstractItemModel {
public:
    enum {GraphRole = Qt::UserRole};
    enum ModelNodeType {
        invalidT,
        nodeT,
        signaltypeT,
        edgeT,
        nodelistT,
        signaltypelistT,
        edgelistT
    };
    struct ModelNode {
        ModelNodeType type;
        QDomElement elem1;
        const QDomElement elem2, elem3;
        ModelNode(ModelNodeType     t,
                  QDomElement &e1,
                  const QDomElement &e2 = QDomElement(),
                  const QDomElement &e3 = QDomElement())
            : type(t)
            , elem1(e1)
            , elem2(e2)
            , elem3(e3){}
        ModelNode()
            : type(invalidT)
            , elem1(QDomElement())
            , elem2(QDomElement())
            , elem3(QDomElement()){}
    };
private:
    typedef list<ModelNode*> ModelNodeList;
public:
    SignalgraphModel(QObject *parent, XmlFileRep *xml_file);
    virtual ~SignalgraphModel();

    virtual QModelIndex index      (int row, int column,
                                    const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent     (const QModelIndex & index) const;
    virtual int         rowCount   (const QModelIndex &parent = QModelIndex()) const;
    virtual int         columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant    data       (const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual bool        hasChildren(const QModelIndex &parent = QModelIndex()) const;

    QModelIndex         index               (ModelNodeType type) const;
    void                updateNodePosition  (const QModelIndex & index, const QPointF &new_position);
private:
    SignalgraphRep signal_graph;
    ModelNode nodesnode, signaltypesnode, edgesnode;
    ModelNodeList nodelist, signaltypelist, edgelist;
};

#endif

