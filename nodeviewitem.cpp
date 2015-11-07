#include "nodeviewitem.h"
#include <iostream>
using namespace std;

#include <QGraphicsSceneMouseEvent>
#include <QMenu>

PortViewItem::PortViewItem(QGraphicsItem *parent, const QColor &_color)
    : QGraphicsItem(parent)
    , color(_color)
    , box(6, 6)
{}

QRectF PortViewItem::boundingRect() const {
    const QPoint frame(1, 1);
    return QRect(-(box + frame) / 2, (box + frame) / 2);
}

void PortViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/){
    QBrush brush(color);
    painter->setBrush(brush);
    painter->drawRect(QRect(-box / 2, box / 2));
}

void PortViewItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    QMenu menu;
    // TODO...
    QAction *removeAction = menu.addAction("Remove");
    QAction *markAction = menu.addAction("Mark");
    QAction *selectedAction = menu.exec(event->screenPos());
    // ...
 }

InPortViewItem::InPortViewItem(QGraphicsItem *parent, int index)
    : PortViewItem(parent, QColor(101,101,255))
{
    setPos(GetInportToplevelPosition(index));
}

OutPortViewItem::OutPortViewItem(QGraphicsItem *parent, int index)
    : PortViewItem(parent, QColor(255,51,51))
{
    setPos(GetOutportToplevelPosition(index));
}







NodeViewItem::NodeViewItem(QDomElement _node, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , box(6, 6)
    , width(100)
    , height(30)
    , node(_node)
{
    setPos(GetNodeToplevelPosition(node));
    int in_count(0), out_count(0);
    if (!node.isNull()){
        //cout << "NodeViewItem " << node.attribute("name").toStdString().c_str() << endl;
        for (QDomElement e=node.firstChildElement(); !e.isNull(); e=e.nextSiblingElement()){
            QGraphicsItem *item(0);
            if (e.nodeName() == QString("intype")){
                item = new InPortViewItem(this, in_count++);
            } else if (e.nodeName() == QString("outtype")){
                item = new OutPortViewItem(this, out_count++);
            }
            if (item){
                item->setToolTip(QString("port: ") + e.attribute("port") + QString("\ntype: ") + e.attribute("type"));
            }
        }
        height += 8 * (in_count > out_count ? in_count : out_count);
    } else {
        height += 8;
        if (node.nodeName() == QString("input")){
            new OutPortViewItem(this);
        } else if (node.nodeName() == QString("output")){
            new InPortViewItem(this);
        }
    }
    setFlag(ItemIsMovable, true);
    //setFlag(ItemIsSelectable, true);
}

QRectF NodeViewItem::boundingRect() const {
    QPoint p_in(GetInportToplevelPosition() - box / 2);
    QPoint p_out(GetOutportToplevelPosition() - box / 2);
    return QRectF(p_in.x(), 0, p_out.x() + box.x() + 5, height + 5);
}

void NodeViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/, QWidget */*widget*/){
    QBrush brush(QColor(255,204,146));
    painter->setBrush(brush);
    painter->drawRoundedRect(0, 0, width, height, 12, 8);
    painter->drawText(10, 16, QString(node.attribute("name")));
}

void NodeViewItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsItem::mouseMoveEvent(event);
    emit positionChanged(this);
}

void NodeViewItem::mousePressEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsItem::mousePressEvent(event);
    setZValue(2);
}

void NodeViewItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsItem::mouseReleaseEvent(event);
    setZValue(0);
}

void NodeViewItem::setNewPosition(const QPoint &new_pos){
    setPos(new_pos);
    emit positionChanged(this);
}




InputProxyViewItem::InputProxyViewItem(QDomElement node, QGraphicsItem *parent)
    : NodeViewItem(node, parent)
    , spot(45, 15, 10, 10) {}

void InputProxyViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QBrush brush(QColor(160,160,255));
    painter->setBrush(brush);
    painter->drawRoundedRect(0, 0, width, height, 12, 8);
    painter->drawText(10, 16, QString(node.attribute("io-type")));
    brush.setColor("white");
    painter->setBrush(brush);
    painter->drawRect(spot);
}

OutputProxyViewItem::OutputProxyViewItem(QDomElement node, QGraphicsItem *parent)
    : NodeViewItem(node, parent)
    , spot(45, 15, 10, 10) {}

void OutputProxyViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    QBrush brush(QColor(160,160,255));
    painter->setBrush(brush);
    painter->drawRoundedRect(0, 0, width, height, 12, 8);
    painter->drawText(10, 16, QString(node.attribute("io-type")));
    brush.setColor("white");
    painter->setBrush(brush);
    painter->drawRect(spot);
}

