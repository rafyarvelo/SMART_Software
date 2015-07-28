#ifndef BCI_C_MCU_IO
#define BCI_C_MCU_IO

#include "bci_c_rvs.h"
#include "../smart_config.h"
#include "../smart_debug_log.h"

class C_Flasher_IO
{
public:
     C_Flasher_IO(C_RVS* pRVS);
    ~C_Flasher_IO();

	static C_Flasher_IO* Instance(C_RVS* pRVS)
	{
		return new C_Flasher_IO(pRVS);
	}
	
    ConnectionStatusType GetConnectionStatus() {return connectionStatus;}
    
private:
	C_RVS* mRVS_Ptr;
    ConnectionStatusType connectionStatus;
};

#endif // BCI_C_MCU_IO

