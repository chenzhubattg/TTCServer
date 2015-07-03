#-------------------------------------------------
#
# Project created by QtCreator 2015-06-10T14:56:04
#
#-------------------------------------------------

#QT       += core gui
#QT       += network
#QT       += testlib
#
#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#
#TARGET = ../TTCbin/TTCServer
#TEMPLATE = app


QT       += core
QT       += network
QT       += testlib

QT       -= gui

TARGET = ../TTCbin/TTCServer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp\
    common.cpp \
    MonitorClient.cpp \
    WatcherClient.cpp \
    MonitorServer.cpp \
    WatcherServer.cpp

HEADERS  += common.h \
    MonitorClient.h \
    WatcherClient.h \
    MonitorServer.h \
    WatcherServer.h

#FORMS    += treewidget.ui
#LIBS += /home/ttc/qttest/build-xmltest-Desktop_Qt_5_4_0_GCC_64bit-Debug/libtinyxml.a
#LIBS += /usr/local/lib/libtinyxml.so
