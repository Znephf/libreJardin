#-------------------------------------------------
#
# Project created by QtCreator 2016-09-01T18:03:06
#
#-------------------------------------------------

QT  += core gui
QT  += xml widgets
QT  += sql widgets
QT  += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = openjardin
TEMPLATE = app


SOURCES += main.cpp\
    myitem.cpp \
    mybutton.cpp \
    database.cpp \
    planning_item.cpp \
    mygrilleitem.cpp \
    mainwindow.cpp \
    mypolygone.cpp \
    myvertex.cpp \
    mypolyline.cpp \
    mygraphicsscene.cpp

HEADERS  += mainwindow.h \
    myitem.h \
    mybutton.h \
    database.h \
    planning_item.h \
    mygrilleitem.h \
    mypolygone.h \
    myvertex.h \
    mypolyline.h \
    mygraphicsscene.h

FORMS    += mainwindow.ui \
    database.ui

RESOURCES += \
    openjardin.qrc

TRANSLATIONS = open-jardin_fr.ts \
               open-jardin_en.ts
