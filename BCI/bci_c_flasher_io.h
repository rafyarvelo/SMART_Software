#ifndef BCI_C_MCU_IO
#define BCI_C_MCU_IO

#include "bci_c_rvs.h"
#include "../smart_config.h"

class C_Flasher_IO
{
public:
     C_Flasher_IO();
    ~C_Flasher_IO();

    ConnectionStatusType GetConnectionStatus() {return connectionStatus;}
    C_RVS&               GetRVSRef()           {return rvs;}
private:
    C_RVS rvs;
    ConnectionStatusType connectionStatus;
};

#endif // BCI_C_MCU_IO

