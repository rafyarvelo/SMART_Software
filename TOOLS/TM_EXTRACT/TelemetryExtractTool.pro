

INCLUDEPATH += ../../
DEPENDPATH  += ../../
INCLUDEPATH += ../../BCI
DEPENDPATH  += ../../BCI
INCLUDEPATH += ../../CCS_PROJECTS
DEPENDPATH  += ../../CCS_PROJECTS

#Copy Executable to debug directory
DESTDIR += .
DESTDIR += ../../debug_files/

HEADERS += \
    ../../smart_config.h \
    ../../smart_data_types.h \
    ../../BCI/bci_c_tm.h \
    ../../CCS_PROJECTS/PCC/power_chair_command_constants.h \
    ../../BCI/bci_c_eeg_data.h \
    ../../BCI/bci_c_abstract_parser.h \
    ../../BCI/bci_c_binary_parser.h \
    ../../BCI/bci_c_textparser.h \
    ../../BCI/bci_c_tm_header_names.h \
    ../../BCI/bci_c_singleton.h

SOURCES += \
    ../../smart_data_types.c \
    ../../BCI/bci_c_eeg_data.cpp \
    ../../BCI/bci_c_tm.cpp \
    ../../BCI/bci_c_abstract_parser.cpp \
    ../../BCI/bci_c_binary_parser.cpp \
    ../../BCI/bci_c_textparser.cpp \
    main.cpp

