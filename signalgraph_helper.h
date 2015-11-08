#ifndef SIGNALGRAPH_HELPER_H
#define SIGNALGRAPH_HELPER_H

#include <QPointF>
#include <QtXml/QDomElement>

class SignalgraphHelper {
protected:
    static QPoint GetGraphToplevelPosition(const QDomElement &graph);
    static void   SetGraphToplevelPosition(QDomElement &graph, const QPoint &new_position);
    static QPoint GetNodeToplevelPosition(const QDomElement &node);
    static void   SetNodeToplevelPosition(QDomElement &node, const QPoint &new_position);
    static QPoint GetInportToplevelPosition(int index = 0);
    static QPoint GetOutportToplevelPosition(int index = 0);
    static int    GetInportIndex(const QDomElement &nodetype, const QString &portname);
    static int    GetOutportIndex(const QDomElement &nodetype, const QString &portname);
};

#endif

