#ifndef BCI_C_MCU_IO
#define BCI_C_MCU_IO

#include "../ssvep/bci_c_rvs.h"

typedef enum
{
    CONNECTION_FAIL,
    CONNECTION_SUCCESS
} ConnectionStatusType;

class C_Flasher_IO
{
public:
     C_Flasher_IO();
    ~C_Flasher_IO();

    ConnectionStatusType GetConnectionStatus() {return connectionStatus;}
    C_RVS&               GetRVSRef() {return rvs;}
private:
    C_RVS rvs;
    ConnectionStatusType connectionStatus;
};

#endif // BCI_C_MCU_IO

