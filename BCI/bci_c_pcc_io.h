#ifndef BCI_C_JA2PCC
#define BCI_C_JA2PCC

#include "../PCC/power_chair_command_constants.h"
#include "bci_c_serial_comm.h"
#include "bci_c_connected_device.h"
#include "../smart_config.h"
#include "../smart_debug_log.h"

class C_PCC_IO : C_ConnectedDevice
{
public:
     C_PCC_IO();
    ~C_PCC_IO();
	
    static C_PCC_IO* Instance() { return new C_PCC_IO;}
	
	void SetCommand(PCC_Command_Type cmd);
	void SendCommand();
	
    virtual ConnectionStatusType connect();

private:
    SMART_DEBUG_LOG* debugLog;
	PCC_Command_Type currentCommand;

    C_Serial_Comm *serialComm;
};

#endif // BCI_C_JA2PCC

