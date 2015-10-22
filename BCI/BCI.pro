
#Added Library from https://github.com/qextserialport/qextserialport
CONFIG += extserialport

#Referenced Headers from other software sections
INCLUDEPATH += ..
INCLUDEPATH += ../CCS_PROJECTS

unix{
INCLUDEPATH += ../../emokit/include/emokit
}

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
    bci_c_eeg_data.h \
    bci_c_abstract_parser.h \
    bci_c_binary_parser.h \
    bci_c_textparser.h \
    bci_c_telemetrymanager.h \
    bci_c_brsh_io_debug.h \
    bci_c_brsh_io_serial.h \
    bci_c_tm_header_names.h \
    bci_c_pcc_io_serial.h \
    bci_c_pcc_io.h \
    bci_c_pcc_io_debug.h \
    bci_c_singleton.h \
    bci_c_framegenerator.h \
    ../smart_data_types.h \
    bci_c_tm.h

    unix{
    HEADERS += bci_c_eeg_io_emotiv.h
    }


SOURCES += \
    bci_c_bci_package.cpp \
    bci_c_eeg_io_debug.cpp \
    bci_c_flasher_io.cpp \
    bci_c_judgment_algorithm.cpp \
    bci_c_rvs.cpp \
    bci_c_serial_comm.cpp \
    bci_c_signal_processing.cpp \
    bci_main.cpp \
    bci_c_eeg_data.cpp \
    bci_c_abstract_parser.cpp \
    bci_c_binary_parser.cpp \
    bci_c_textparser.cpp \
    bci_c_telemetrymanager.cpp \
    bci_c_brsh_io_debug.cpp \
    bci_c_brsh_io_serial.cpp \
    bci_c_pcc_io_serial.cpp \
    bci_c_pcc_io_debug.cpp \
    bci_c_brsh_io.cpp \
    bci_c_eeg_io.cpp \
    bci_c_framegenerator.cpp \
    ../smart_data_types.c \
    bci_c_pcc_io.cpp \
    bci_c_tm.cpp

    unix{
    SOURCES += bci_c_eeg_io_emotiv.cpp
    }

#Crap Needed for Emokit libraries
unix{
    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../emokit/lib/release/ -lemokit
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../emokit/lib/debug/ -lemokit
    else:symbian: LIBS += -lemokit
    else:unix: LIBS += -L$$PWD/../../emokit/lib/ -lemokit

    INCLUDEPATH += $$PWD/../../emokit/include
    DEPENDPATH += $$PWD/../../emokit/include

    win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../emokit/lib/release/emokit.lib
    else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../emokit/lib/debug/emokit.lib
    else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../../emokit/lib/libemokit.a

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/release/ -lmcrypt
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/debug/ -lmcrypt
    else:unix: LIBS += -L$$PWD/../../../../../usr/lib/ -lmcrypt

    INCLUDEPATH += $$PWD/../../../../../usr/include
    DEPENDPATH += $$PWD/../../../../../usr/include

    win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/release/libmcrypt.a
    else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/debug/libmcrypt.a
    else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/release/mcrypt.lib
    else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/debug/mcrypt.lib
    else:unix: PRE_TARGETDEPS += $$PWD/../../../../../usr/lib/libmcrypt.a

    win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/release/ -lhidapi-hidraw
    else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/local/lib/debug/ -lhidapi-hidraw
    else:unix: LIBS += -L$$PWD/../../../../../usr/local/lib/ -lhidapi-hidraw

    INCLUDEPATH += $$PWD/../../../../../usr/local/include
    DEPENDPATH += $$PWD/../../../../../usr/local/include

    win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/release/libhidapi-hidraw.a
    else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/debug/libhidapi-hidraw.a
    else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/release/hidapi-hidraw.lib
    else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/debug/hidapi-hidraw.lib
    else:unix: PRE_TARGETDEPS += $$PWD/../../../../../usr/local/lib/libhidapi-hidraw.a
}

#Crap Needed for Qextserialport on windows
win32{
 LIBS += -L$$PWD/../../qextserialport/release/ -lQt5ExtSerialPort1
 INCLUDEPATH += $$PWD/../../qextserialport/src/
 DEPENDPATH += $$PWD/../../qextserialport/src/
}
