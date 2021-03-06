#-------------------------------------------------
#
# Project created by QtCreator 2013-10-13T21:08:12
#
#-------------------------------------------------

QT       += core gui
QT       += xml
QT       += network
QT       += webkit
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
RC_FILE += hello.rc
TARGET = TimeRecord5th
TEMPLATE = app


SOURCES += main.cpp\
    readrecord.cpp \
    form.cpp \
    content_check.cpp \
    smsreader.cpp \
    recordgetandpost.cpp \
    mainrecorddb.cpp \
    sorttherecordnamelikedir.cpp

HEADERS  += \
    readrecord.h \
    form.h \
    globe.h \
    content_check.h \
    smsreader.h \
    recordgetandpost.h \
    mainrecorddb.h \
    sorttherecordnamelikedir.h

FORMS    += \
    readrecord.ui \
    form.ui \
    mainrecorddb.ui \
    sorttherecordnamelikedir.ui

OTHER_FILES +=
