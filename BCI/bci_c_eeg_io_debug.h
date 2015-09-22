#ifndef EEG_IO_DEBUG
#define EEG_IO_DEBUG

#include <cstdlib>
#include <ctime>
#include "bci_c_eeg_io.h"
#include "bci_c_framegenerator.h"

class C_EEG_IO_DEBUG : public C_EEG_IO , public C_Singleton<C_EEG_IO_DEBUG>
{
    Q_OBJECT
public:
             C_EEG_IO_DEBUG(){}
    virtual ~C_EEG_IO_DEBUG(){}

    virtual eegTypeEnum  getType() { return EEG_TYPE_DEBUG; }
    virtual ConnectionStatusType connect() {return CONNECTED;}

public slots:
    virtual bool fetchEEGFrame();
};

#endif

