#ifndef GRAPHHINTS_REP_H
#define GRAPHHINTS_REP_H

#include "xmlfile_rep.h"
#include <QPoint>
#include <QRect>
#include <list>

using namespace std;

class GH_Node;
class SignalgraphScene;
class DeploymentScene;

class GraphHintsRep: public XmlFileRep {
public:
    GraphHintsRep(const QString &filename);
    GraphHintsRep(const SignalgraphScene *model);
    GraphHintsRep(const DeploymentScene *scene);
    virtual ~GraphHintsRep();
    QPoint getNodeHintPos    (const QString &nodename,    const QString &mode=QString()) const;
    QPoint getInProxyHintPos (const QString &processname, const QString &sourcename, const QString &iotypename, const QString &mode=QString()) const;
    QPoint getOutProxyHintPos(const QString &processname, const QString &destname, const QString &iotypename, const QString &mode=QString()) const;
    QRect  getProcessHintRect(const QString &processname, const QString &mode=QString()) const;
private:
    GH_Node       *getProcessHint(const QString &processname, const QString &mode);
    const GH_Node *getProcessHint(const QString &processname, const QString &mode) const;
    GraphHintsRep(const GraphHintsRep&);
    GraphHintsRep &operator=(const GraphHintsRep&);
    list<GH_Node*> node_list;
    QString mode;
};

#endif

