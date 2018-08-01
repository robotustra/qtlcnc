#-------------------------------------------------
#
# Project created by QtCreator 2018-07-30T20:47:45
#
#-------------------------------------------------

QT       += core gui opengl widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtlcnc
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    layoutdata.cpp \
    state.cpp \
    mybutton.cpp \
    lcs.cpp \
    mesh.cpp \
    backdrop.cpp \
    glmesh.cpp \
    myindicator.cpp \
    simplelayout.cpp

HEADERS  += mainwindow.h \
    layoutdata.h \
    state.h \
    mybutton.h \
    lcs.h \
    mesh.h \
    backdrop.h \
    glmesh.h \
    myindicator.h \
    simplelayout.h

FORMS    += mainwindow.ui

CONFIG += c++11
