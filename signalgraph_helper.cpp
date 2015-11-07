#include "signalgraph_helper.h"
#include <QString>


QPoint SignalgraphHelper::GetGraphToplevelPosition(const QDomElement &graph){
    QPoint pos(0.0, 0.0);
    QDomElement g;
    for (g=graph.firstChildElement(); !g.isNull(); g=g.nextSiblingElement()){
        if (g.nodeName() == QString("graph-hint")) {
            const QString a = g.attribute("mode");
            if (a.isNull() || (a == QString("toplevel"))){
                break;
            }
        }
    }
    if (!g.isNull()){
        pos.setX(g.attribute("x").toInt());
        pos.setY(g.attribute("y").toInt());
    }
    return pos;
}

void    SignalgraphHelper::SetGraphToplevelPosition(QDomElement &graph, const QPoint &new_position){
    QDomElement g;
    for (g=graph.firstChildElement(); !g.isNull(); g=g.nextSiblingElement()){
        if (g.nodeName() == QString("graph-hint")){
            const QString a = g.attribute("mode");
            if (a.isNull() || (a == QString("toplevel"))){
                break;
            }
        }
    }
    if (!g.isNull()) {
        g.setAttribute("x", QString("%1").arg((int)new_position.x()).toStdString().c_str());
        g.setAttribute("y", QString("%1").arg((int)new_position.y()).toStdString().c_str());
    } else {
        // TODO: neues graph-hint Element anlegen.
    }
}

QPoint SignalgraphHelper::GetNodeToplevelPosition(const QDomElement &node){
    QPoint pos(0, 0);
    QDomElement g;
    for (g=node.firstChildElement(); !g.isNull(); g=g.nextSiblingElement()){
        if (g.nodeName() == QString("graph-hint")) {
            const QString a = g.attribute("mode");
            if (a.isNull() || (a == QString("toplevel"))){
                break;
            }
        }
    }
    if (!g.isNull()){
        pos.setX(g.attribute("x").toInt());
        pos.setY(g.attribute("y").toInt());
    }
    return pos;
}

void    SignalgraphHelper::SetNodeToplevelPosition(QDomElement &node, const QPoint &new_position){
    QDomElement g;
    for (g=node.firstChildElement(); !g.isNull(); g=g.nextSiblingElement()){
        if (g.nodeName() == QString("graph-hint")){
            const QString a = g.attribute("mode");
            if (a.isNull() || (a == QString("toplevel"))){
                break;
            }
        }
    }
    if (!g.isNull()) {
        g.setAttribute("x", QString("%1").arg((int)new_position.x()).toStdString().c_str());
        g.setAttribute("y", QString("%1").arg((int)new_position.y()).toStdString().c_str());
    } else {
        // TODO: neues graph-hint Element anlegen.
    }
}

static qreal getY_offs(int index){
    return 15.5 + 8 * index;
}

static qreal getInportX_offs(void){
    return 0;
}

static qreal getOutportX_offs(void){
    return 100;
}

QPoint SignalgraphHelper::GetInportToplevelPosition(int index){
    return QPoint(getInportX_offs(), getY_offs(index));
}

QPoint SignalgraphHelper::GetOutportToplevelPosition(int index){
    return QPoint(getOutportX_offs(), getY_offs(index));
}

int     SignalgraphHelper::GetInportIndex(const QDomElement &nodetype, const QString &portname){
    int index(0);
    if (!portname.isNull() && !nodetype.isNull()){
        for (QDomElement e=nodetype.firstChildElement(); !e.isNull(); e=e.nextSiblingElement()){
            if (e.nodeName() == QString("intype")){
                if (e.attribute("port") == portname){
                    break;
                }
                ++index;
            }
        }
    }
    return index;
}

int     SignalgraphHelper::GetOutportIndex(const QDomElement &nodetype, const QString &portname){
    int index(0);
    if (!portname.isNull() && !nodetype.isNull()){
        for (QDomElement e=nodetype.firstChildElement(); !e.isNull(); e=e.nextSiblingElement()){
            if (e.nodeName() == QString("outtype")){
                if (e.attribute("port") ==  portname){
                    break;
                }
                ++index;
            }
        }
    }
    return index;
}



