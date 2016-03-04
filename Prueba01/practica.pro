#-------------------------------------------------
#
# Project created by QtCreator 2015-09-15T18:32:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PAG-1-3b
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mainglview.cpp \
    pagshaderprogram.cpp \
    pagcamera.cpp \
    pagrevolutionobject.cpp \
    ipolacion.cpp

HEADERS  += mainwindow.h \
    mainglview.h \
    pagshaderprogram.h \
    pagcamera.h \
    pagrevolutionobject.h \
    ipolacion.h

FORMS    += mainwindow.ui

DISTFILES += \
    points-1.frag \
    points-1.vert \
    lines-1.frag \
    lines-1.vert \
    phong.frag \
    phong.vert

RESOURCES += \
    resources.qrc
