#-------------------------------------------------
#
# Project created by QtCreator 2015-06-14T20:46:44
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = BCI_Package
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

HEADERS += \
    common/bci_c_bci_package.h \
    common/bci_c_signal_processing.h \
    JA/bci_c_judgment_algorithm.h \
    JA/bci_c_ja2brs.h \
    JA/bci_c_ja2pcc.h \
    common/bci_c_tm.h \
    io/bci_c_eeg_io.h \
    ssvep/bci_c_rvs.h \
    ssvep/bci_c_rvs2ja.h \
    io/bci_c_flasher_io.h
