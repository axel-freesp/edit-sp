#ifndef SIGNALGRAPH_REP_H
#define SIGNALGRAPH_REP_H


#include "xmlfile_rep.h"
#include <list>
using namespace std;

//class SG_Node;
//class QDomDocument;
//class QDomElement;

class SignalgraphRep {
public:
    SignalgraphRep(XmlFileRep *xml_file);
    virtual ~SignalgraphRep();
    int nodesCount() const {return n_nodes;}
    int edgesCount() const {return n_edges;}
    QDomElement get_rootElement() {return xml_rep->get_rootElement();}
private:
    SignalgraphRep(const SignalgraphRep&);
    SignalgraphRep &operator=(const SignalgraphRep&);
    XmlFileRep *xml_rep;
    int n_nodes;
    int n_edges;
};

#endif

