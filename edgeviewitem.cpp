#include "edgeviewitem.h"
#include "nodeviewitem.h"
#include <iostream>
using namespace std;
#include <math.h>

#include <QGraphicsSceneMouseEvent>
#include "processviewitem.h"

EdgeViewItem::EdgeViewItem(const NodeViewItem *_from, const NodeViewItem *_to, int _from_index, int _to_index, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , from(_from)
    , to(_to)
    , from_index(_from_index)
    , to_index(_to_index)
    , from_pos(from->pos() + GetOutportToplevelPosition(from_index))
    , to_pos(to->pos() + GetInportToplevelPosition(to_index))
{
    setPos(from_pos);
}

QRectF EdgeViewItem::boundingRect() const {
    qreal penWidth = 1;
    qreal left   = min(from_pos.x(), to_pos.x() - 4);
    qreal right  = max(from_pos.x(), to_pos.x() + 4);
    qreal top    = min(from_pos.y(), to_pos.y() - 4);
    qreal bottom = max(from_pos.y(), to_pos.y() + 4);
    QPointF topleft(left - penWidth / 2, top - penWidth / 2);
    QSizeF size(right - left + penWidth, bottom - top + penWidth);
    return QRectF(topleft - pos(), size);
}

void EdgeViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/){
    const QPointF apex(to_pos - pos());
    const QPointF d(to_pos - from_pos);
    const qreal r(sqrt(d.x() * d.x() + d.y() * d.y()));
    if (r){
        const qreal s(10);
        const QPointF h0(apex.x() - s * d.x() / r, apex.y() - s * d.y() / r);
        const QPointF h1(apex.x() - s * d.y() / r, apex.y() + s * d.x() / r);
        const QPointF h2(apex.x() + s * d.y() / r, apex.y() - s * d.x() / r);
        //painter->setPen(color);
        painter->drawLine(QPointF(), apex);
        painter->drawLine((2 * h0 + h1) / 3, apex);
        painter->drawLine((2 * h0 + h2) / 3, apex);
    }
}

void EdgeViewItem::redrawEdge(const NodeViewItem *item){
    //cout << ATTR(item->node, "name") << " redrawEdge: " << new_position.x() << "," << new_position.y() << endl;
    if (item == from){
        hide();
        from_pos = item->pos() + GetOutportToplevelPosition(from_index);
        setPos(from_pos);
        show();
    }
    if (item == to){
        hide();
        to_pos = item->pos() + GetInportToplevelPosition(to_index);
        show();
    }
}



ProxyEdgeViewItem::ProxyEdgeViewItem(const ProcessViewItem *_src, const ProcessViewItem *_dst, const NodeViewItem *_from, const NodeViewItem *_to, QGraphicsItem *parent)
        : QGraphicsItem(parent)
        , shift(50, 20)
        , from(_from)
        , to(_to)
        , src_pos(_src->pos())
        , dst_pos(_dst->pos())
        , from_pos(from->pos())
        , to_pos(to->pos()){
    setPos(src_pos + from_pos + shift);
}

void ProxyEdgeViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/){
    const QPointF apex(dst_pos + to_pos + shift - pos());
    const QPointF d(dst_pos + to_pos - src_pos - from_pos);
    const qreal r(sqrt(d.x() * d.x() + d.y() * d.y()));
    if (r){
        const qreal s(10);
        const QPointF h0(apex.x() - s * d.x() / r, apex.y() - s * d.y() / r);
        const QPointF h1(apex.x() - s * d.y() / r, apex.y() + s * d.x() / r);
        const QPointF h2(apex.x() + s * d.y() / r, apex.y() - s * d.x() / r);
        painter->setPen("red");
        painter->drawLine(QPointF(), apex);
        painter->drawLine((2 * h0 + h1) / 3, apex);
        painter->drawLine((2 * h0 + h2) / 3, apex);
    }
}

QRectF ProxyEdgeViewItem::boundingRect() const {
    qreal penWidth = 1;
    qreal left   = min(src_pos.x() + from_pos.x(), dst_pos.x() + to_pos.x() - 4);
    qreal right  = max(src_pos.x() + from_pos.x(), dst_pos.x() + to_pos.x() + 4);
    qreal top    = min(src_pos.y() + from_pos.y(), dst_pos.y() + to_pos.y() - 4);
    qreal bottom = max(src_pos.y() + from_pos.y(), dst_pos.y() + to_pos.y() + 4);
    QPointF topleft(left - penWidth / 2, top - penWidth / 2);
    QSizeF size(right - left + penWidth, bottom - top + penWidth);
    return QRectF(topleft - pos() + shift, size);
}

void ProxyEdgeViewItem::redrawEdge(const NodeViewItem *item){
    if (item == from){
        hide();
        from_pos = item->pos();
        setPos(src_pos + from_pos + shift);
        show();
    }
    if (item == to){
        hide();
        to_pos = item->pos();
        show();
    }
}

void ProxyEdgeViewItem::redrawEdge(const ProcessViewItem *item){
    for (list<NodeViewItem*>::const_iterator i=item->nodeviewitemlist.begin(); i!=item->nodeviewitemlist.end(); ++i){
        if ((*i) == from){
            hide();
            src_pos = item->pos();
            setPos(src_pos + from_pos + shift);
            show();
        }
        if ((*i) == to){
            hide();
            dst_pos = item->pos();
            show();
        }
    }
}

