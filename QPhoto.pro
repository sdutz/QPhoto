#-------------------------------------------------
#
# Project created by QtCreator 2011-05-01T19:47:42
#
#-------------------------------------------------

QT       += core gui
QT       +=  phonon

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
    musicmgr.cpp

HEADERS  += cphoto.h \
    aboutdlg.h \
    photoview.h \
    confmgr.h \
    settingsdlg.h \
    errordlg.h \
    util.h \
    musicmgr.h

FORMS    += cphoto.ui \
    aboutdlg.ui \
    settingsdlg.ui \
    errordlg.ui \
    musicmgr.ui
