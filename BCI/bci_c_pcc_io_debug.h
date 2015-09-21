#ifndef BCI_C_PCC_IO_DEBUG_H
#define BCI_C_PCC_IO_DEBUG_H

#include "bci_c_pcc_io.h"

class C_PCC_IO_Debug : public C_PCC_IO , public C_Singleton<C_PCC_IO_Debug>
{
public:
    C_PCC_IO_Debug();
    virtual ~C_PCC_IO_Debug(){}

    virtual void SendCommand()
    {
        cout << "Sending PCC Command: " << currentCommand << endl;
    }

    virtual ConnectionStatusType connect() { return CONNECTED; }
};

#endif // BCI_C_PCC_IO_DEBUG_H
