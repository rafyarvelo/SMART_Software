
#Added Library from https://github.com/qextserialport/qextserialport
include(api/qextserialport/src/qextserialport.pri)

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
    bci_c_pcc_io.h

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
    bci_c_pcc_io.cpp
