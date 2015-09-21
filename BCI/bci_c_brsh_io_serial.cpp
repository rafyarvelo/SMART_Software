#include "bci_c_brsh_io_serial.h"

C_BRSH_IO_Serial::C_BRSH_IO_Serial()
{
    //Initialize Serial Communication
    mSerialCommPtr = C_Serial_Comm::Instance(QString(BRS_PORT));
}

C_BRSH_IO_Serial::~C_BRSH_IO_Serial()
{

}

//Send TM to the BRSH
void C_BRSH_IO_Serial::SendTMFrame(TM_Frame_t* pFrame)
{

}

bool C_BRSH_IO_Serial::fetchBRSFrame()
{
    return false;
}

ConnectionStatusType C_BRSH_IO_Serial::connect()
{
    ConnectionStatusType status = NOT_CONNECTED;

    return status;
}
