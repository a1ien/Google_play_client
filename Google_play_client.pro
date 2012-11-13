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
<<<<<<< HEAD
    emptysettingswarning.cpp
=======
    downloader.cpp
>>>>>>> 8e74a52da19ed27e20e4fbeb12ce86621b52127a

HEADERS  += mainwindow.h \
    settings.h \
    marketsession.h \
    market.pb.h \
<<<<<<< HEAD
    emptysettingswarning.h
=======
    downloader.h
>>>>>>> 8e74a52da19ed27e20e4fbeb12ce86621b52127a

FORMS    += mainwindow.ui \
    settings.ui \
    emptysettingswarning.ui

OTHER_FILES += \
    market.proto
LIBS += -lprotobuf
