#ifndef BCI_C_PCC_IO_DEBUG_H
#define BCI_C_PCC_IO_DEBUG_H

#include "bci_c_pcc_io.h"

class C_PCC_IO_Debug : public C_PCC_IO , public C_Singleton<C_PCC_IO_Debug>
{
public:
    C_PCC_IO_Debug();
    virtual ~C_PCC_IO_Debug(){}

public slots:
    virtual void SendCommand(PCC_Command_Type cmd);
    virtual void EmergencyStop(){ SendCommand(PCC_STOP);}
    virtual ConnectionStatusType connect() { return CONNECTED; }
};

#endif // BCI_C_PCC_IO_DEBUG_H
