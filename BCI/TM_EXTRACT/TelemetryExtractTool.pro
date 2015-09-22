
INCLUDEPATH += ../
DEPENDPATH  += ../
INCLUDEPATH += ../../
DEPENDPATH  += ../../
INCLUDEPATH += ../../BRS
DEPENDPATH  += ../../BRS
INCLUDEPATH += ../../PCC
DEPENDPATH  += ../../PCC

#Copy Executable to debug directory
DESTDIR += .
DESTDIR += ../../debug_files/

HEADERS += \
    ../bci_c_tm.h \
    ../../smart_config.h \
    ../../smart_debug_log.h \
    ../../PCC/power_chair_command_constants.h \
    ../bci_c_eeg_data.h \
    ../../BRS/brs_c_sensor_data.h \
    ../bci_c_abstract_parser.h \
    ../bci_c_binary_parser.h \
    ../bci_c_textparser.h \
    ../bci_c_tm_header_names.h \
    ../bci_c_singleton.h

SOURCES += \
    ../bci_c_eeg_data.cpp \
    ../bci_c_tm.cpp \
    ../bci_c_abstract_parser.cpp \
    ../bci_c_binary_parser.cpp \
    ../bci_c_textparser.cpp \
    main.cpp

