#ifndef PROCESSVIEWITEM_H
#define PROCESSVIEWITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QObject>
#include "signalgraph_helper.h"
#include <list>
using namespace std;

class NodeViewItem;

class ResizeViewItem: public QObject, public QGraphicsItem {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    ResizeViewItem(QGraphicsItem *parent);
    virtual QRectF boundingRect() const;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
    const QColor color;
    const QPoint box;
signals:
    void positionChanged(ResizeViewItem *item);
    void mouseRelease(ResizeViewItem *item);
};

class ProcessViewItem: public QObject, public QGraphicsItem, public SignalgraphHelper {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    ProcessViewItem(QDomElement _graph, const QColor &_color, QGraphicsItem *parent=0);
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void         setNewPosition(const QPoint &new_pos);
    void         setNewRect(const QRect &new_pos);
    int          left()   const {return ((pos() + resizer_topleft->pos())).x();}
    int          top()    const {return ((pos() + resizer_topleft->pos())).y();}
    int          width()  const {return (resizer_bottomright->pos() - resizer_topleft->pos()).x();}
    int          height() const {return (resizer_bottomright->pos() - resizer_topleft->pos()).y();}
signals:
    void positionChanged(const ProcessViewItem *self);  // connect to edges
    void newPosition(const ProcessViewItem *self);      // connect to parent->resize
private slots:
    void sizeChanged(ResizeViewItem *item);
    void mouseRelease(ResizeViewItem *item);
    void positionChanged(const NodeViewItem *item);
public:
    QDomElement graph;
private:
    const QColor color;
    QPoint box_topleft, box_bottomright, old_box_topleft, old_box_bottomright;
    const QPoint min_box;
    ResizeViewItem *resizer_topleft, *resizer_bottomright;
public:
    list<NodeViewItem*> nodeviewitemlist;
};

#endif
