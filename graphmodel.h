#ifndef GRAPHMODEL_H
#define GRAPHMODEL_H

#include <QAbstractItemModel>
#include "signalgraph_rep.h"
//#include "components_rep.h"

class GraphModel: public QAbstractItemModel {
public:
    enum {GraphRole = Qt::UserRole};
public:
    GraphModel(QObject *parent, const QString &graph_filename);
    virtual ~GraphModel();

    virtual QModelIndex index      (int row, int column,
                                    const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent     (const QModelIndex & index) const;
    virtual int         rowCount   (const QModelIndex &parent = QModelIndex()) const;
    virtual int         columnCount(const QModelIndex &parent) const;
    virtual QVariant    data       (const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual bool        hasChildren(const QModelIndex &parent = QModelIndex()) const;
public:
    //QString ComponentsFilename() const;
private:
    SignalgraphRep signal_graph;
    //ComponentsRep *components;
    //DOMElement *nodes, *nodetypes;
};

#endif

