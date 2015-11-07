#ifndef NODEVIEWITEM_H_
#define NODEVIEWITEM_H_

#include <QGraphicsItem>
#include <QPainter>
#include <QObject>
#include "signalgraph_helper.h"

class PortViewItem: public QObject, public QGraphicsItem, public SignalgraphHelper {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    PortViewItem(QGraphicsItem *parent, const QColor &_color);
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
protected:
    const QColor color;
    const QPoint box;
};

class InPortViewItem: public PortViewItem {
public:
    InPortViewItem(QGraphicsItem *parent, int index = 0);
};

class OutPortViewItem: public PortViewItem {
public:
    OutPortViewItem(QGraphicsItem *parent, int index = 0);
};

class NodeViewItem: public QObject, public QGraphicsItem, public SignalgraphHelper {
    Q_OBJECT
public:
    NodeViewItem(QDomElement node, QGraphicsItem *parent = 0);
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void         setNewPosition(const QPoint &new_pos);
protected:
    const QPoint box;
public:
    int width, height;
    QDomElement node;
signals:
    void positionChanged(const NodeViewItem *self);
};

class InputProxyViewItem: public NodeViewItem{
    const QRect spot;
public:
    InputProxyViewItem(QDomElement node, QGraphicsItem *parent = 0);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

class OutputProxyViewItem: public NodeViewItem{
    const QRect spot;
public:
    OutputProxyViewItem(QDomElement node, QGraphicsItem *parent = 0);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif

