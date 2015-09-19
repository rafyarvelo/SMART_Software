#ifndef BCI_C_BRSH_IO_SERIAL_H
#define BCI_C_BRSH_IO_SERIAL_H

#include "bci_c_brsh_io.h"
#include "bci_c_serial_comm.h"

class C_BRSH_IO_Serial : public C_BRSH_IO , public C_Singleton<C_BRSH_IO_Serial>
{
public:
    C_BRSH_IO_Serial();
    virtual ~C_BRSH_IO_Serial();

    static C_BRSH_IO_Serial* Instance() { return new C_BRSH_IO_Serial; }

    //Send TM to the BRSH
    virtual void SendTMFrame(TM_Frame_t* pFrame);

    //Return the latest BRS Data
    virtual BRS_Frame_t* GetLatestBRSFrame();

    virtual ConnectionStatusType connect();
private:
    C_Serial_Comm* mSerialCommPtr;
};

#endif // BCI_C_BRSH_IO_SERIAL_H
