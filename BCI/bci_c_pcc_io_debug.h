#ifndef BCI_C_PCC_IO_DEBUG_H
#define BCI_C_PCC_IO_DEBUG_H

#include "bci_c_pcc_io.h"

class C_PCC_IO_Debug : public C_PCC_IO
{
public:
    C_PCC_IO_Debug();
    virtual ~C_PCC_IO_Debug(){}

    static C_PCC_IO_Debug* Instance() {return new C_PCC_IO_Debug; }
    virtual void SendCommand() { }

    virtual ConnectionStatusType connect() { return CONNECTED; }
};

#endif // BCI_C_PCC_IO_DEBUG_H
