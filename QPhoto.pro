#-------------------------------------------------
#
# Project created by QtCreator 2011-05-01T19:47:42
#
#-------------------------------------------------

QT       += core gui
QT       +=  phonon
QT       += core sql

TARGET = QPhoto
TEMPLATE = app

SOURCES += main.cpp\
        cphoto.cpp \
    aboutdlg.cpp \
    photoview.cpp \
    confmgr.cpp \
    settingsdlg.cpp \
    errordlg.cpp \
    util.cpp \
    musicmgr.cpp \
    collectionmgr.cpp \
    collectiondlg.cpp

HEADERS  += cphoto.h \
    aboutdlg.h \
    photoview.h \
    confmgr.h \
    settingsdlg.h \
    errordlg.h \
    util.h \
    musicmgr.h \
    collectionmgr.h \
    collectiondlg.h \
    macro.h

FORMS    += cphoto.ui \
    aboutdlg.ui \
    settingsdlg.ui \
    errordlg.ui \
    musicmgr.ui \
    collectiondlg.ui

TRANSLATIONS += QPhoto_it.ts \
     QPhoto_en.ts
