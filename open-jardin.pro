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

TARGET = open_jardin
TEMPLATE = app


SOURCES += main.cpp\
    myitem.cpp \
    #test.cpp
    database.cpp \
    planning_item.cpp \
    mygrilleitem.cpp \
    mainwindow.cpp

HEADERS  += mainwindow.h \
    myitem.h \
    database.h \
    planning_item.h \
    mygrilleitem.h

FORMS    += mainwindow.ui \
    database.ui

RESOURCES += \
    supervision.qrc

TRANSLATIONS = open-jardin_fr.ts \
               open-jardin_en.ts
