#ifndef _GRAPH_HINT_NODE_H_
#define _GRAPH_HINT_NODE_H_

#include <QString>
#include <QRect>
#include <QtXml/QDomElement>

class ProcessViewItem;
class NodeViewItem;

class GH_Node {
public:
    QDomElement node;
    QRect      box;
    QString     mode;
protected:
    GH_Node(QDomElement _node, const QRect &_box, const QString &_mode);
    GH_Node(QDomElement _node);
public:
    virtual ~GH_Node(){}
            QPoint  pos()                   const {return box.topLeft();}
            void    setPos(const QPoint &p)       {box.moveTopLeft(p);}
            QString getMode()               const {return mode;}
            qreal   getWidth()              const {return box.width();}
            qreal   getHeight()             const {return box.height();}
    virtual QString getProcess()            const = 0;
    virtual QString getName()               const = 0;
    virtual QString getIOType()             const = 0;
    virtual QString getSource()             const = 0;
    virtual QString getDest()               const = 0;
};

class GH_FrameNode: public GH_Node {
public:
    GH_FrameNode(QDomElement _node, const QRect &_box, const QString &_mode = QString());
    GH_FrameNode(QDomElement _node);
    virtual QString getProcess() const {return QString();}
    virtual QString getName()    const {return QString();}
    virtual QString getIOType()  const {return QString();}
    virtual QString getSource()  const {return QString();}
    virtual QString getDest()    const {return QString();}
};

class GH_ProcessNode: public GH_Node {
public:
    QString     process;
    GH_ProcessNode(QDomElement _node, const ProcessViewItem *item, const QString &_mode = QString(), const QRect &bbox = QRect());
    GH_ProcessNode(QDomElement _node);
    virtual QString getProcess() const {return process;}
    virtual QString getName()    const {return QString();}
    virtual QString getIOType()  const {return QString();}
    virtual QString getSource()  const {return QString();}
    virtual QString getDest()    const {return QString();}
};

class GH_PNodeNode: public GH_Node {
public:
    QString     name;
    GH_PNodeNode(QDomElement _node, const NodeViewItem *item, const QString &mode = QString(), const ProcessViewItem *parent = 0);
    GH_PNodeNode(QDomElement _node);
    virtual QString getProcess() const {return QString();}
    virtual QString getName()    const {return name;}
    virtual QString getIOType()  const {return QString();}
    virtual QString getSource()  const {return QString();}
    virtual QString getDest()    const {return QString();}
};

class GH_InProxyNode: public GH_Node {
public:
    QString process, source, iotype;
    GH_InProxyNode(QDomElement _node, const NodeViewItem *item, const QString &mode = QString(), const ProcessViewItem *parent = 0);
    GH_InProxyNode(QDomElement _node);
    virtual QString getProcess() const {return process;}
    virtual QString getName()    const {return QString();}
    virtual QString getIOType()  const {return iotype;}
    virtual QString getSource()  const {return source;}
    virtual QString getDest()    const {return QString();}
};

class GH_OutProxyNode: public GH_Node {
public:
    QString process, dest, iotype;
    GH_OutProxyNode(QDomElement _node, const NodeViewItem *item, const QString &mode = QString(), const ProcessViewItem *parent = 0);
    GH_OutProxyNode(QDomElement _node);
    virtual QString getProcess() const {return process;}
    virtual QString getName()    const {return QString();}
    virtual QString getIOType()  const {return iotype;}
    virtual QString getSource()  const {return QString();}
    virtual QString getDest()    const {return dest;}
};

#endif
