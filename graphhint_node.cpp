#include "graphhint_node.h"
#include "processviewitem.h"
#include "nodeviewitem.h"
#include <QDebug>

static const int g_border(10);

GH_Node::GH_Node(QDomElement _node)
    : node(_node)
    , box(node.attribute("x").toDouble(), node.attribute("y").toDouble(), node.attribute("w").toDouble(), node.attribute("h").toDouble())
    , mode(node.attribute("mode"))
{}

GH_Node::GH_Node(QDomElement _node, const QRect &bbox, const QString &_mode)
    : node(_node)
    , box(bbox)
    , mode(_mode)
{
    node.setAttribute("mode", mode);
}

GH_FrameNode::GH_FrameNode(QDomElement _node, const QRect &_box, const QString &_mode)
    : GH_Node(_node, _box, _mode)
{
    node.setAttribute("w", box.width());
    node.setAttribute("h", box.height());
}

GH_FrameNode::GH_FrameNode(QDomElement _node)
    : GH_Node(_node, QRect(_node.attribute("x").toDouble(),
                            _node.attribute("y").toDouble(),
                            _node.attribute("w").toDouble(),
                            _node.attribute("h").toDouble()),
                     _node.attribute("mode"))
{}

GH_ProcessNode::GH_ProcessNode(QDomElement _node, const ProcessViewItem *item, const QString &_mode, const QRect &bbox)
    : GH_Node(_node, QRect(item->left() - bbox.x() + g_border, item->top() - bbox.y() + g_border, item->width(), item->height()), _mode)
    , process(item->graph.attribute("process"))
{
    node.setAttribute("process", process);
    node.setAttribute("deployment-color", "rgb(200, 200, 200)");
    node.setAttribute("x", box.x());
    node.setAttribute("y", box.y());
    node.setAttribute("w", box.width());
    node.setAttribute("h", box.height());
}

GH_ProcessNode::GH_ProcessNode(QDomElement _node)
    : GH_Node(_node, QRect(_node.attribute("x").toDouble(),
                            _node.attribute("y").toDouble(),
                            _node.attribute("w").toDouble(),
                            _node.attribute("h").toDouble()),
                     _node.attribute("mode"))
    , process(_node.attribute("process"))
{
}

GH_PNodeNode::GH_PNodeNode(QDomElement _node, const NodeViewItem *item, const QString &_mode, const ProcessViewItem *parent)
    : GH_Node(_node, QRect(item->x() - ((parent != 0) ? parent->left() - parent->x() : 0), item->y() - ((parent != 0) ? parent->top() - parent->y() : 0), item->width, item->height), _mode)
    , name(item->node.attribute("name"))
{
    node.setAttribute("name", name);
    node.setAttribute("x", box.x());
    node.setAttribute("y", box.y());
}

GH_PNodeNode::GH_PNodeNode(QDomElement _node)
    : GH_Node(_node)
    , name(_node.attribute("name"))
{}

GH_InProxyNode::GH_InProxyNode(QDomElement _node, const NodeViewItem *item, const QString &_mode, const ProcessViewItem *parent)
    : GH_Node(_node, QRect(item->x() - ((parent != 0) ? parent->left() - parent->x() : 0), item->y() - ((parent != 0) ? parent->top() - parent->y() : 0), item->width, item->height), _mode)
    , process(static_cast<ProcessViewItem*>(item->parentItem())->graph.attribute("process"))
    , source(item->node.attribute("source"))
    , iotype(item->node.attribute("io-type"))
{
    node.setAttribute("process", process);
    node.setAttribute("source", source);
    node.setAttribute("io-type", iotype);
    node.setAttribute("x", box.x());
    node.setAttribute("y", box.y());
}

GH_InProxyNode::GH_InProxyNode(QDomElement _node)
    : GH_Node(_node)
    , process(_node.attribute("process"))
    , source(_node.attribute("source"))
    , iotype(_node.attribute("io-type"))
{}

GH_OutProxyNode::GH_OutProxyNode(QDomElement _node, const NodeViewItem *item, const QString &_mode, const ProcessViewItem *parent)
    : GH_Node(_node, QRect(item->x() - ((parent != 0) ? parent->left() - parent->x() : 0), item->y() - ((parent != 0) ? parent->top() - parent->y() : 0), item->width, item->height), _mode)
    , process(static_cast<ProcessViewItem*>(item->parentItem())->graph.attribute("process"))
    , dest(item->node.attribute("dest"))
    , iotype(item->node.attribute("io-type"))
{
    node.setAttribute("process", process);
    node.setAttribute("dest", dest);
    node.setAttribute("io-type", iotype);
    node.setAttribute("x", box.x());
    node.setAttribute("y", box.y());
}

GH_OutProxyNode::GH_OutProxyNode(QDomElement _node)
    : GH_Node(_node)
    , process(_node.attribute("process"))
    , dest(_node.attribute("dest"))
    , iotype(_node.attribute("io-type"))
{}

