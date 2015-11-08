QT += widgets xmlpatterns
TEMPLATE = app
FORMS = frontend.ui
SOURCES = main.cpp \
    mainwindow.cpp \
    xmlfile_rep.cpp \
    signalgraph_rep.cpp \
    graphhints_rep.cpp \
    graphhint_node.cpp \
    signalgraph_model.cpp \
    nodeviewitem.cpp \
    edgeviewitem.cpp \
    signalgraph_scene.cpp \
    signalgraph_helper.cpp \
    processviewitem.cpp \
    deploymentscene.cpp
HEADERS = mainwindow.h \
    xmlfile_rep.h \
    signalgraph_rep.h \
    graphhints_rep.h \
    graphhint_node.h \
    signalgraph_model.h \
    nodeviewitem.h \
    edgeviewitem.h \
    signalgraph_scene.h \
    signalgraph_helper.h \
    processviewitem.h \
    deploymentscene.h
INCLUDEPATH =
LIBS = -lQt5Xml
