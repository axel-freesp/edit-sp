#ifndef _EDGEVIEWITEM_H_
#define _EDGEVIEWITEM_H_

#include <QGraphicsItem>
#include <QPainter>
#include <QObject>
#include "signalgraph_helper.h"
#include <list>

class NodeViewItem;
class ProcessViewItem;

class EdgeViewItem: public QObject, public QGraphicsItem, public SignalgraphHelper {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    EdgeViewItem(const NodeViewItem *_from, const NodeViewItem *_to, int _from_index, int _to_index, QGraphicsItem *parent = 0);
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
public slots:
    void redrawEdge(const NodeViewItem *item);
protected:
    const NodeViewItem *from, *to;
    int from_index, to_index;
    QPointF from_pos, to_pos;
};

class ProxyEdgeViewItem: public QObject, public QGraphicsItem, public SignalgraphHelper {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    ProxyEdgeViewItem(const ProcessViewItem *_src, const ProcessViewItem *_dst, const NodeViewItem *_from, const NodeViewItem *_to, QGraphicsItem *parent = 0);
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
public slots:
    void redrawEdge(const NodeViewItem *item);
    void redrawEdge(const ProcessViewItem *item);
private:
    const QPointF shift;
    const NodeViewItem *from, *to;
    QPointF src_pos, dst_pos;
    QPointF from_pos, to_pos;
};

#endif

