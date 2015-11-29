QMAKE_CC = gcc-4.8
QMAKE_CXX = g++-4.8

#Added Library from https://github.com/qextserialport/qextserialport
CONFIG += extserialport

#Referenced Headers from other software sections
INCLUDEPATH += ..
INCLUDEPATH += ../CCS_PROJECTS

unix{
INCLUDEPATH += ../../emokit/include/emokit
}

#C++11 Support 
QMAKE_CXXFLAGS += -std=c++11

HEADERS += \
    bci_c_bci_package.h \
    bci_c_safequeue.h \
    bci_c_eeg_io.h \
    bci_c_eeg_io_debug.h \
    bci_c_eeg_io_nautilus.h \
    bci_c_flasher_io.h \
    bci_c_judgment_algorithm.h \
    bci_c_rvs.h \
    bci_c_serial_comm.h \
    bci_c_signal_processing.h \
    ../smart_config.h \
    ../smart_debug_log.h \
    ../CCS_PROJECTS/PCC/power_chair_command_constants.h \
    bci_c_connected_device.h \
    bci_c_brsh_io.h \
    bci_c_abstract_parser.h \
    bci_c_binary_parser.h \
    bci_c_textparser.h \
    bci_c_telemetrymanager.h \
    bci_c_brsh_io_debug.h \
    bci_c_brsh_io_serial.h \
    bci_c_tm_header_names.h \
    bci_c_portnames.h \
    bci_c_pcc_io_serial.h \
    bci_c_pcc_io.h \
    bci_c_pcc_io_debug.h \
    bci_c_singleton.h \
    bci_c_framegenerator.h \
    ../smart_data_types.h \
    bci_c_flasher_io_debug.h \
    bci_c_flasher_io_gpio.h \
    seniordesign.pb.h


SOURCES += \
    bci_c_bci_package.cpp \
    bci_c_eeg_io_debug.cpp \
    bci_c_flasher_io.cpp \
    bci_c_judgment_algorithm.cpp \
    bci_c_rvs.cpp \
    bci_c_serial_comm.cpp \
    bci_c_signal_processing.cpp \
    bci_main.cpp \
    bci_c_abstract_parser.cpp \
    bci_c_binary_parser.cpp \
    bci_c_textparser.cpp \
    bci_c_telemetrymanager.cpp \
    bci_c_brsh_io_debug.cpp \
    bci_c_brsh_io_serial.cpp \
    bci_c_pcc_io_serial.cpp \
    bci_c_pcc_io_debug.cpp \
    bci_c_portnames.cpp \
    bci_c_brsh_io.cpp \
    bci_c_eeg_io.cpp \
    bci_c_framegenerator.cpp \
    ../smart_data_types.c \
    bci_c_pcc_io.cpp \
    bci_c_flasher_io_debug.cpp \
    bci_c_flasher_io_gpio.cpp \
    seniordesign.pb.cc

#Crap Needed for Qextserialport on windows
win32{
 LIBS += -L$$PWD/../../qextserialport/release/ -lQt5ExtSerialPort1
 INCLUDEPATH += $$PWD/../../qextserialport/src/
 DEPENDPATH += $$PWD/../../qextserialport/src/
}

#Crap Needed for communications with signal processing daemon
LIBS += -lzmq -lprotobuf
