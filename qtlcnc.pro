#-------------------------------------------------
#
# Project created by QtCreator 2018-07-30T20:47:45
#
#-------------------------------------------------

QT       += core gui opengl widgets network

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
    simplelayout.cpp \
    path.cpp \
    layoutobject.cpp \
    myglview.cpp \
    gcodeview.cpp \
    gcodeedit.cpp

HEADERS  += mainwindow.h \
    layoutdata.h \
    state.h \
    mybutton.h \
    lcs.h \
    mesh.h \
    backdrop.h \
    glmesh.h \
    myindicator.h \
    simplelayout.h \
    path.h \
    layoutobject.h \
    myglview.h \
    gcodeview.h \
    gcodeedit.h

FORMS    += mainwindow.ui

CONFIG += c++11

LIBS += -LC:\Qt\4.8.7\lib\ -lopengl32 -LC:\minwg32\lib
