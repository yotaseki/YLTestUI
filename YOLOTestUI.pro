#-------------------------------------------------
#
# Project created by QtCreator 2018-11-15T22:53:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

TARGET = YOLOTestUI
TEMPLATE = app


SOURCES += main.cpp\
        YOLOTestUI.cpp \
    detect.cpp \
    MyQclass.cpp \
    readtxt.cpp \
    test.cpp

HEADERS  += YOLOTestUI.h \
    detect.h \
    MyQclass.h \
    readtxt.h \
    test.h

FORMS    += YOLOTestUI.ui


LIBS += -ldarknet -lopencv_core -lopencv_highgui -lopencv_imgproc
INCLUDEPATH += -I/usr/include/opencv
