#-------------------------------------------------
#
# Project created by QtCreator 2018-03-17T15:03:53
#
#-------------------------------------------------

QT       += core gui sql
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = probeControl
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    dbmanager.cpp \
    crc.cpp \
    serial.cpp \
    srvrclntcomm.cpp \
    settingdialog.cpp \
    editablesqlmodel.cpp \
    controldelegate.cpp \
    keypad.cpp \
    passwddiag.cpp \
    keyeventfilter.cpp

HEADERS  += mainwindow.h \
    dbmanager.h \
    crc.h \
    serial.h \
    srvrclntcomm.h \
    settingdialog.h \
    editablesqlmodel.h \
    controldelegate.h \
    keypad.h \
    passwddiag.h \
    keyeventfilter.h

FORMS    += mainwindow.ui \
    settingdialog.ui \
    keypad.ui \
    passwddiag.ui

INCLUDEPATH += $$PWD/libmodbus-3.0.6/src

LIBS += -L$$PWD/libmodbus-3.0.6/src/.libs/static -lmodbus
