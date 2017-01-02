#-------------------------------------------------
#
# Project created by QtCreator 2017-01-01T12:48:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SimpleSlideshow
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    ImageWidget.cpp \
    SlideshowSetting.cpp \
    Playlist.cpp

HEADERS  += MainWindow.h \
    ImageWidget.h \
    SlideshowSetting.h \
    Playlist.h

FORMS    += MainWindow.ui \
    ImageWidget.ui
