#-------------------------------------------------
#
# Project created by QtCreator 2014-07-18T09:45:41
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = GameView
TEMPLATE = app
RC_FILE = mainicon.rc


SOURCES += main.cpp\
        GameView.cpp \
    GameField.cpp \
    mainwindow.cpp

HEADERS  += GameView.h \
    GameField.h \
    mainwindow.h

FORMS    += \
    mainwindow.ui

CONFIG += mobility
MOBILITY = 

RESOURCES += \
    resources.qrc
