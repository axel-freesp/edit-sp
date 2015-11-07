#ifndef _XML_FILE_REP_H_
#define _XML_FILE_REP_H_

#include <QtXml/QDomDocument>
#include <iostream>

class XmlFileRep{
protected:
    QDomDocument domDocument;
public:
    XmlFileRep(const QString &filename);
    XmlFileRep();
    virtual ~XmlFileRep();
    QDomElement get_rootElement();
    friend std::ostream &operator<<(std::ostream&, const XmlFileRep&);
private:
    XmlFileRep(const XmlFileRep&);
    XmlFileRep &operator=(const XmlFileRep&);
};

#endif
