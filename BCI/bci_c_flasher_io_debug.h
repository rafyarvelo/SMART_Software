#ifndef C_FLASHER_IO_DEBUG_H
#define C_FLASHER_IO_DEBUG_H

#include "bci_c_flasher_io.h"

class C_Flasher_IO_Debug : public C_Flasher_IO, public C_Singleton<C_Flasher_IO_Debug>
{
public:
    C_Flasher_IO_Debug();
    virtual ~C_Flasher_IO_Debug();

public slots:
    virtual ConnectionStatusType connect();
    virtual void SendRVS();
};

#endif // C_FLASHER_IO_DEBUG_H
