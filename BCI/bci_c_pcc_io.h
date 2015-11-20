#ifndef BCI_C_PCC_IO_H
#define BCI_C_PCC_IO_H

#include <QObject>
#include "../CCS_PROJECTS/PCC/power_chair_command_constants.h"
#include "bci_c_connected_device.h"
#include "bci_c_singleton.h"
#include "../smart_data_types.h"
#include "../smart_debug_log.h"

class C_PCC_IO : public QObject , public C_ConnectedDevice
{
    Q_OBJECT
public:
    C_PCC_IO();
    virtual ~C_PCC_IO();

    virtual ConnectionStatusType connect() = 0;


public:
    PCC_Command_Type GetPrevCommand() { return prevCommand; }

public slots:
    virtual void SendCommand(PCC_Command_Type cmd) = 0;
    virtual void EmergencyStop() = 0;

protected:
    SMART_DEBUG_LOG* debugLog;
    PCC_Command_Type prevCommand;
};

#endif // BCI_C_PCC_IO_H
