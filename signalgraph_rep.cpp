#include "signalgraph_rep.h"
#include <iostream>
#include <string.h>
#include <list>
#include <QtXml/QDomDocument>
#include <QFile>

using namespace std;

SignalgraphRep::SignalgraphRep(XmlFileRep *xml_file)
    : xml_rep(xml_file)
    , n_nodes(0)
    , n_edges(0)
{
    QDomElement r = get_rootElement();
    if (!r.isNull()){
        QDomElement nodes = r.firstChildElement("nodes");
        if (!nodes.isNull()){
            for (QDomElement n=nodes.firstChildElement(); !n.isNull(); n=n.nextSiblingElement()){
                ++n_nodes;
            }
        }
        nodes = r.firstChildElement("connections");
        if (!nodes.isNull()){
            for (QDomElement n=nodes.firstChildElement(); !n.isNull(); n=n.nextSiblingElement()){
                ++n_edges;
            }
        }
    }
}

SignalgraphRep::~SignalgraphRep(){}


