#-------------------------------------------------
#
# Project created by QtCreator 2012-11-12T11:54:26
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Google_play_client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settings.cpp \
    marketsession.cpp \
    market.pb.cc \
    downloader.cpp \
    decompressor.cpp

HEADERS  += mainwindow.h \
    settings.h \
    marketsession.h \
    market.pb.h \
    downloader.h \
    decompressor.h

FORMS    += mainwindow.ui \
    settings.ui

OTHER_FILES += \
    market.proto
LIBS += -lprotobuf

RESOURCES += \
    resource.qrc
