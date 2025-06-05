QT += testlib widgets
QT -= gui

CONFIG += console c++11
CONFIG -= app_bundle

SOURCES += test_util.cpp \
           ../utilitaires/util.cpp \
           ../graphic/background.cpp \
           ../consts.cpp

INCLUDEPATH += ../utilitaires \
               ../graphic \
               ..
