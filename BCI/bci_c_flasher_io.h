#ifndef BCI_C_MCU_IO
#define BCI_C_MCU_IO

#include "bci_c_rvs.h"
#include "bci_c_connected_device.h"
#include "../smart_config.h"
#include "../smart_debug_log.h"

class C_Flasher_IO : C_ConnectedDevice
{
public:
     C_Flasher_IO(C_RVS* pRVS);
    ~C_Flasher_IO();

	static C_Flasher_IO* Instance(C_RVS* pRVS)
	{
		return new C_Flasher_IO(pRVS);
	}
	
    virtual ConnectionStatusType connect() {return CONNECTED;}
    void SendRVS();

private:
	C_RVS* mRVS_Ptr;
};

#endif // BCI_C_MCU_IO

