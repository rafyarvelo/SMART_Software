include(../BCI.pro)

INCLUDEPATH += ../
DEPENDPATH  += ../
INCLUDEPATH += ../../
DEPENDPATH  += ../../
INCLUDEPATH += ../../emokit/include/emokit/
DEPENDPATH  += ../../emokit/include/emokit/

#Copy Executable to debug directory
DESTDIR += .
DESTDIR += ../../debug_files/

SOURCES += \
    main.cpp
