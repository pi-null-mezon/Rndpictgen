#-------------------------------------------------
#
# Project created by QtCreator 2015-09-03T10:35:33
#
#-------------------------------------------------

QT       += core \
            gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

TARGET = randpicture
VERSION =   1.0.0.0
DEFINES +=  APP_NAME=\\\"$${TARGET}\\\" \
            APP_VERS=\\\"$${VERSION}\\\" \
            APP_DESIGNER=\\\"Alex.A.Taranov\\\"

SOURCES += main.cpp
