#-------------------------------------------------
#
# Project created by QtCreator 2015-06-16T22:10:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = project
TEMPLATE = app


SOURCES += main.cpp\
    classwizard.cpp \
    qexcel.cpp \
    mainwidget.cpp \
    mainwindow.cpp \
    mdichild.cpp

HEADERS  += \
    classwizard.h \
    qexcel.h \
    mainwidget.h \
    mainwindow.h \
    mdichild.h

FORMS    += mainwindow.ui

CONFIG  += axcontainer

#LIBS += -l.\Qt5AxBased
#LIBS += -l.\Qt5AxContainerd

include(xlsx/qtxlsx.pri)

RESOURCES += \
    mdi.qrc
