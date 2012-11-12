#-------------------------------------------------
#
# Project created by QtCreator 2012-11-12T11:54:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Google_play_client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settings.cpp \
    marketsession.cpp \
    market.pb.cc

HEADERS  += mainwindow.h \
    settings.h \
    marketsession.h \
    market.pb.h

FORMS    += mainwindow.ui \
    settings.ui

OTHER_FILES += \
    market.proto
