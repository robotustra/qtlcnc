#-------------------------------------------------
#
# Project created by QtCreator 2018-07-30T20:47:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtlcnc
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    layoutdata.cpp \
    state.cpp

HEADERS  += mainwindow.h \
    layoutdata.h \
    state.h

FORMS    += mainwindow.ui
