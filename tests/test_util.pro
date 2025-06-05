QT += testlib
QT -= gui

CONFIG += console c++11
CONFIG -= app_bundle

SOURCES += test_util.cpp \
           ../utilitaires/util.cpp

INCLUDEPATH += ../utilitaires
