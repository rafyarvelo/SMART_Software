#ifndef BCI_C_JA2PCC
#define BCI_C_JA2PCC

#include "../PCC/power_chair_command_constants.h"
#include "bci_c_serial_comm.h"
#include "bci_c_pcc_io.h"

class C_PCC_IO_Serial : public C_PCC_IO, public C_Singleton<C_PCC_IO_Serial>
{
public:
     C_PCC_IO_Serial();
    ~C_PCC_IO_Serial();
	
    //Implementation of virtual functions
    virtual void SendCommand();
    virtual ConnectionStatusType connect();

private:

    //Serial Communication API
    C_Serial_Comm *serialComm;
};

#endif // BCI_C_JA2PCC
