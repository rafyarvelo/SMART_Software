#ifndef BCI_C_BRSH_IO_SERIAL_H
#define BCI_C_BRSH_IO_SERIAL_H

#include "bci_c_brsh_io.h"
#include "bci_c_serial_comm.h"
#include "smart_data_types.h"

class C_BRSH_IO_Serial : public C_BRSH_IO ,
                         public C_Singleton<C_BRSH_IO_Serial>
{
public:
    C_BRSH_IO_Serial();
    virtual ~C_BRSH_IO_Serial();

    //Send TM to/from the BRSH
    virtual bool fetchBRSFrame();
    virtual void SendTMFrame(tmFrameBufferType* tmFrameBuffer);

    virtual ConnectionStatusType connect();

private slots:
    void onPortDisconnected();

private:
    C_Serial_Comm* mSerialPortPtr;
};

#endif // BCI_C_BRSH_IO_SERIAL_H
