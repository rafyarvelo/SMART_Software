#ifndef C_BRSH_IO_H
#define C_BRSH_IO_H

#include "../smart_config.h"
#include "../smart_debug_log.h"
#include "bci_c_connected_device.h"
#include "bci_c_tm.h"

class C_BRSH_IO : public C_ConnectedDevice
{
public:
     C_BRSH_IO();
    ~C_BRSH_IO();

    static C_BRSH_IO* Instance(){ return new C_BRSH_IO; }

    C_TM* GetLatestTM();
    void  SendTM();

    virtual ConnectionStatusType connect() { return CONNECTED; }

private:
    C_TM* tmDataBuffer;
};

#endif // C_BRSH_IO_H
