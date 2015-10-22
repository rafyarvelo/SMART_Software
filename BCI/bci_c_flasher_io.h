#ifndef BCI_C_MCU_IO
#define BCI_C_MCU_IO

#include "bci_c_rvs.h"
#include "bci_c_connected_device.h"
#include "bci_c_singleton.h"
#include "../smart_config.h"
#include "../smart_debug_log.h"

class C_Flasher_IO : public C_ConnectedDevice, public C_Singleton<C_Flasher_IO>
{
public:
     C_Flasher_IO();
    ~C_Flasher_IO();

    virtual ConnectionStatusType connect();
    void SendRVS(C_RVS* pRVS);
};

#endif // BCI_C_MCU_IO

