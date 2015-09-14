
#Added Library from https://github.com/qextserialport/qextserialport
CONFIG += extserialport

#Referenced Headers from other software sections
INCLUDEPATH += ..
INCLUDEPATH += ../PCC

HEADERS += \
    bci_c_bci_package.h \
    bci_c_eeg_io.h \
    bci_c_eeg_io_debug.h \
    bci_c_eeg_io_emotiv.h \
    bci_c_eeg_io_nautilus.h \
    bci_c_flasher_io.h \
    bci_c_judgment_algorithm.h \
    bci_c_rvs.h \
    bci_c_serial_comm.h \
    bci_c_signal_processing.h \
    bci_c_tm.h \
    ../smart_config.h \
    ../smart_debug_log.h \
    ../PCC/power_chair_command_constants.h \
    bci_c_connected_device.h \
    bci_c_brsh_io.h \
    bci_c_pcc_io.h \
    bci_c_eeg_data.h

SOURCES += \
    bci_c_bci_package.cpp \
    bci_c_eeg_io_debug.cpp \
    bci_c_flasher_io.cpp \
    bci_c_judgment_algorithm.cpp \
    bci_c_rvs.cpp \
    bci_c_serial_comm.cpp \
    bci_c_signal_processing.cpp \
    bci_main.cpp \
    bci_c_brsh_io.cpp \
    bci_c_pcc_io.cpp \
    bci_c_eeg_io_emotiv.cpp \
    bci_c_eeg_data.cpp

#Junk needed for Emokit Libraries
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../emokit/lib/release/ -lemokit
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../emokit/lib/debug/ -lemokit
else:unix: LIBS += -L$$PWD/../emokit/lib/ -lemokit

INCLUDEPATH += $$PWD/../emokit/include/emokit
DEPENDPATH += $$PWD/../emokit/include/emokit

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../emokit/lib/release/libemokit.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../emokit/lib/debug/libemokit.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../emokit/lib/release/emokit.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../emokit/lib/debug/emokit.lib
else:unix: PRE_TARGETDEPS += $$PWD/../emokit/lib/libemokit.a

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
