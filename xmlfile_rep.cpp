#include "xmlfile_rep.h"
#include <QFile>
#include <iostream>
using namespace std;

XmlFileRep::XmlFileRep(const QString &filename){
    QFile   file(filename);
    QString error;
    int     errline;
    bool succ = domDocument.setContent((QIODevice*)&file, &error, &errline);
    if (!succ){
        cout << "SignalgraphRep error in file " << filename.toStdString().c_str()
             << " line " << errline << ": " << error.toStdString().c_str() << endl;
    }
}

XmlFileRep::XmlFileRep(){
}

XmlFileRep::~XmlFileRep(){
}

QDomElement XmlFileRep::get_rootElement(){
    return domDocument.documentElement();
}

ostream &operator<<(ostream &o, const XmlFileRep &x){
    return o << (const char*) x.domDocument.toByteArray(4);
}
