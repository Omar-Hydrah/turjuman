#-------------------------------------------------
#
# Project created by QtCreator 2017-12-24T16:03:09
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = turjuman
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp\
        connection.cpp

HEADERS  += dialog.h\
		connection.h

FORMS    += dialog.ui
