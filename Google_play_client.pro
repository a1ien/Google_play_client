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
    emptysettingswarning.cpp\
    downloader.cpp

HEADERS  += mainwindow.h \
    settings.h \
    marketsession.h \
    market.pb.h \
    emptysettingswarning.h \
    downloader.h

FORMS    += mainwindow.ui \
    settings.ui \
    emptysettingswarning.ui

OTHER_FILES += \
    market.proto
LIBS += -lprotobuf
