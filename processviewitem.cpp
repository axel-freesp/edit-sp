#include "processviewitem.h"
#include "nodeviewitem.h"
#include "edgeviewitem.h"
#include <QDebug>

ResizeViewItem::ResizeViewItem(QGraphicsItem *parent)
        : QGraphicsItem(parent)
        , color(QColor(255,255,255))
        , box(6, 6){
    setFlag(ItemIsMovable, true);
}

QRectF ResizeViewItem::boundingRect() const {
    const QPointF frame(1, 1);
    return QRectF(-(box + frame) / 2, (box + frame) / 2);
}

void ResizeViewItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsItem::mouseMoveEvent(event);
    emit positionChanged(this);
}

void ResizeViewItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsItem::mouseReleaseEvent(event);
    emit mouseRelease();
}

void ResizeViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/){
    QBrush brush(color);
    painter->setBrush(brush);
    painter->drawRect(QRectF(-box / 2, box / 2));
}



ProcessViewItem::ProcessViewItem(QDomElement _graph, const QColor &_color, QGraphicsItem *parent)
        : QGraphicsItem(parent)
        , graph(_graph)
        , color(_color)
        , box_topleft(-140, -140)
        , box_bottomright(140, 140)
        , old_box_topleft(box_topleft)
        , old_box_bottomright(box_bottomright)
        , min_box(120, 80){
    setPos(GetGraphToplevelPosition(graph));
    resizer_topleft     = new ResizeViewItem(this);
    resizer_bottomright = new ResizeViewItem(this);
    resizer_topleft->setPos(box_topleft);
    resizer_bottomright->setPos(box_bottomright);
    connect(resizer_topleft, SIGNAL(positionChanged(ResizeViewItem*)),
            this, SLOT(sizeChanged(ResizeViewItem*)));
    connect(resizer_bottomright, SIGNAL(positionChanged(ResizeViewItem*)),
            this, SLOT(sizeChanged(ResizeViewItem*)));
    connect(resizer_topleft, SIGNAL(mouseRelease(ResizeViewItem*)),
            this, SLOT(mouseRelease(ResizeViewItem*)));
    connect(resizer_bottomright, SIGNAL(mouseRelease(ResizeViewItem*)),
            this, SLOT(mouseRelease(ResizeViewItem*)));
    setFlag(ItemIsMovable, true);
    QDomElement nodes = graph.firstChildElement("nodes");
    for (QDomElement n=nodes.firstChildElement(); !n.isNull(); n=n.nextSiblingElement()){
        NodeViewItem *item;
        QString nodename = n.attribute("name");
        if (nodename.startsWith("input_proxy_")){
            item = new InputProxyViewItem(n, this);
        } else if (nodename.startsWith("output_proxy_")){
            item = new OutputProxyViewItem(n, this);
        } else {
            item = new NodeViewItem(n, this);
        }
        item->setPos(-item->width/2, -item->height/2);
        nodeviewitemlist.push_back(item);
        connect(item, SIGNAL(positionChanged(const NodeViewItem*)),
                this, SLOT(positionChanged(const NodeViewItem*)));
    }
    QDomElement edges = graph.firstChildElement("connections");
    for (QDomElement edge=edges.firstChildElement(); !edge.isNull(); edge=edge.nextSiblingElement()){
        const NodeViewItem *from = 0, *to = 0;
        for (list<NodeViewItem*>::const_iterator j=nodeviewitemlist.begin(); j!=nodeviewitemlist.end(); ++j){
            if ((*j)->node.attribute("name") == edge.attribute("from")){
                from = *j;
            }
            if ((*j)->node.attribute("name") == edge.attribute("to")){
                to = *j;
            }
        }
        if ((from != 0) && (to != 0)){
            int from_index = GetOutportIndex(from->node, edge.attribute("from-port"));
            int to_index   = GetInportIndex(to->node, edge.attribute("to-port"));
            //cout << ", index: " << from_index << ", " << to_index << ", elem: " << hex << (void*)node->elem2 << ", " << (void*)node->elem3 << dec << endl;
            EdgeViewItem *item = new EdgeViewItem(from, to, from_index, to_index, this);
            for (list<NodeViewItem*>::const_iterator nvi=nodeviewitemlist.begin(); nvi!=nodeviewitemlist.end(); ++nvi){
                if (   ((*nvi)->node.attribute("name") == edge.attribute("from"))
                    || ((*nvi)->node.attribute("name") == edge.attribute("to"))){
                    connect((*nvi), SIGNAL(positionChanged(const NodeViewItem*)),
                            item, SLOT(redrawEdge(const NodeViewItem*)));
                }
            }
        }
    }
}

QRectF ProcessViewItem::boundingRect() const {
    const QPointF frame(1, 1);
    QRectF r = QRectF(old_box_topleft, old_box_bottomright).united(QRectF(box_topleft, box_bottomright));
    return QRectF(r.topLeft() - frame/2, r.bottomRight() + frame/2);
}

void ProcessViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/){
    QBrush brush(color);
    painter->setBrush(brush);
    painter->drawRect(QRectF(box_topleft, box_bottomright));
    painter->drawText(box_topleft + QPointF(10, 16), QString(graph.attribute("process")));
    old_box_topleft     = box_topleft;
    old_box_bottomright = box_bottomright;
}

void ProcessViewItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsItem::mouseMoveEvent(event);
    emit positionChanged(this);
}

void ProcessViewItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsItem::mouseReleaseEvent(event);
    emit newPosition(this);
}

void ProcessViewItem::setNewPosition(const QPoint &new_pos){
    //prepareGeometryChange();
    setPos(new_pos);
    emit positionChanged(this);
}

void ProcessViewItem::setNewRect(const QRect &new_rect){
    //prepareGeometryChange();
    old_box_topleft = box_topleft;
    old_box_bottomright = box_bottomright;
    setPos(new_rect.x(), new_rect.y());
    resizer_topleft->setPos(QPoint());
    resizer_bottomright->setPos(QPoint(new_rect.width(), new_rect.height()));
    box_topleft     = QPoint();
    box_bottomright = QPoint(new_rect.width(), new_rect.height());
    emit positionChanged(this);
}

void ProcessViewItem::sizeChanged(ResizeViewItem *item){
    //prepareGeometryChange();
    if (item == resizer_bottomright){
        old_box_bottomright = box_bottomright;
        box_bottomright    = item->pos().toPoint();
        QPoint shape = box_bottomright - box_topleft;
        if (shape.x() < min_box.x()){
            box_bottomright.setX(box_topleft.x() + min_box.x());
            resizer_bottomright->setPos(box_topleft + min_box);
        }
        if (shape.y() < min_box.y()){
            box_bottomright.setY(box_topleft.y() + min_box.y());
            resizer_bottomright->setPos(box_topleft + min_box);
        }
    } else if (item == resizer_topleft){
        old_box_topleft = box_topleft;
        box_topleft     = item->pos().toPoint();
        QPoint shape = box_bottomright - box_topleft;
        if (shape.x() < min_box.x()){
            box_topleft.setX(box_bottomright.x() - min_box.x());
            resizer_topleft->setPos(box_bottomright - min_box);
        }
        if (shape.y() < min_box.y()){
            box_topleft.setY(box_bottomright.y() - min_box.y());
            resizer_topleft->setPos(box_bottomright - min_box);
        }
    }
    update();
    emit positionChanged(this);
}

void ProcessViewItem::mouseRelease(){
    emit newPosition(this);
}

void ProcessViewItem::positionChanged(const NodeViewItem *item){
}
