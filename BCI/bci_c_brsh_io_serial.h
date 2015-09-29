#ifndef BCI_C_BRSH_IO_SERIAL_H
#define BCI_C_BRSH_IO_SERIAL_H

#include "bci_c_brsh_io.h"
#include "bci_c_serial_comm.h"
#include "../CCS_PROJECTS/BRS/brs_c_message_constants.h"

class C_BRSH_IO_Serial : public C_BRSH_IO , public C_Singleton<C_BRSH_IO_Serial>
{
public:
    C_BRSH_IO_Serial();
    virtual ~C_BRSH_IO_Serial();

    //Send TM to/from the BRSH
    virtual bool fetchBRSFrame();
    virtual void SendTMFrame(TM_Frame_t* pFrame);

    virtual ConnectionStatusType connect();
private:
    C_Serial_Comm* mSerialCommPtr;
};

#endif // BCI_C_BRSH_IO_SERIAL_H
