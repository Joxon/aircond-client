#-------------------------------------------------
#
# Project created by QtCreator 2018-05-18T15:15:47
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rooms
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    quiwidget.cpp

HEADERS += \
    mainwindow.h \
    quiwidget.h \
    IconsFontAwesome5.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    res/qss.qrc \
    res/main.qrc

DISTFILES += \
    uncrustify.cfg
