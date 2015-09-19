#ifndef BCI_C_PCC_IO_H
#define BCI_C_PCC_IO_H

#include "../PCC/power_chair_command_constants.h"
#include "bci_c_connected_device.h"
#include "bci_c_singleton.h"
#include "../smart_config.h"
#include "../smart_debug_log.h"

class C_PCC_IO : public C_ConnectedDevice
{
public:
     C_PCC_IO();
     virtual ~C_PCC_IO();

    void SetCommand(PCC_Command_Type cmd);
    virtual void SendCommand() = 0;

    virtual ConnectionStatusType connect() = 0;

protected:
    SMART_DEBUG_LOG* debugLog;
    PCC_Command_Type currentCommand;
};

#endif // BCI_C_PCC_IO_H
