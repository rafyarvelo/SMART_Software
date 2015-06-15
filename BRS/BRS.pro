#-------------------------------------------------
#
# Project created by QtCreator 2015-06-14T21:28:44
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = BRS
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

HEADERS += \
    brs_c_brs_package.h \
    brs_c_brs_processing.h \
    brs_c_brs2ja.h \
    brs_c_abstract_sensor.h \
    brs_c_gps_sensor.h \
    brs_c_us_sensor.h \
    brs_c_sensor_io.h \
    brs_c_mobile_device_io.h
