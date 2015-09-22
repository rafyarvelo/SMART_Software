#ifndef BCI_C_PCC_IO_H
#define BCI_C_PCC_IO_H

#include <QObject>
#include "../PCC/power_chair_command_constants.h"
#include "bci_c_connected_device.h"
#include "bci_c_singleton.h"
#include "../smart_config.h"
#include "../smart_debug_log.h"

class C_PCC_IO : public QObject , public C_ConnectedDevice
{
    Q_OBJECT
public:
     C_PCC_IO(){}
     virtual ~C_PCC_IO(){}

    void SetCommand(PCC_Command_Type cmd) { currentCommand = cmd; }
    virtual ConnectionStatusType connect() = 0;

public slots:
    virtual void SendCommand() = 0;
    void SendCommand(PCC_Command_Type& cmd) { SetCommand(cmd); SendCommand(); }

protected:
    SMART_DEBUG_LOG* debugLog;
    PCC_Command_Type currentCommand;
};

#endif // BCI_C_PCC_IO_H
