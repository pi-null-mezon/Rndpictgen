#-------------------------------------------------
#
# Project created by QtCreator 2015-09-03T10:35:33
#
#-------------------------------------------------

QT       += core \
            gui

CONFIG   += console
CONFIG   -= app_bundle

TARGET = randpicture
VERSION =   1.0.0.0

DEFINES +=  APP_NAME=\\\"$${TARGET}\\\" \
            APP_VERS=\\\"$${VERSION}\\\" \
            APP_DESIGNER=\\\"Alex.A.Taranov\\\"

TEMPLATE = app

SOURCES += main.cpp


CONFIG(release, debug|release): DEFINES += QT_NO_WARNING_OUTPUT
