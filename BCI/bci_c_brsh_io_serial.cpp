#include "bci_c_brsh_io_serial.h"

C_BRSH_IO_Serial::C_BRSH_IO_Serial()
    : C_Serial_Comm(BRS_PORT)
{
}

C_BRSH_IO_Serial::~C_BRSH_IO_Serial()
{

}

//Send TM to the BRSH
void C_BRSH_IO_Serial::SendTMFrame(TM_Frame_t* pFrame)
{
    unsigned int msgID = BCI2BRS_MSG_ID;

    //Write the TM Frame to the BRS through the UART Serial Port
    sendToSerialPort(msgID);
    sendToSerialPort(pFrame);
}

bool C_BRSH_IO_Serial::fetchBRSFrame()
{
    bool received      = false;
    unsigned int msgID = 0;
    sizeType  msgSize  = 0;
    u_int64_t bytesAvailable = mSerialPort->bytesAvailable();

    //Don't Bother if the data isn't there yet
    if (bytesAvailable < (sizeof(BRS_Frame_t) + sizeof(msgID)))
    {
        return false;
    }

    //Flush the Port if we're getting full
    else if (bytesAvailable >= MAX_BYTES_AVAILABLE)
    {
        mSerialPort->readAll();
    }

    int i = 0;

    //Look for the Message ID one byte at a time
    while (bytesAvailable-- >= sizeof(BRS_Frame_t))
    {
        unsigned char byte;
        unsigned int  bitsToShift;

        //Read a Byte from the UART
        readFromSerialPort(&byte);

        //Overwrite one byte in the current MsgID
        bitsToShift = (8 * (i++ % sizeof(msgID)));
        msgID &= ~(0xFF) << bitsToShift;
        msgID |=   byte  << bitsToShift;

        //Try to read BRS Frame
        if (msgID == BRS2BCI_MSG_ID)
        {
            debugLog->println(BRS_LOG, "MSG ID Received: BRS2BCI MSG ID\n");

            //Read in the Frame
            pBRSFrameMutex->acquire(BRS_FRAME_MUTEX);
            readFromSerialPort(pLatestBRSFrame);
            received = true;
            emit BRSFrameReceived(pLatestBRSFrame);
            pBRSFrameMutex->release(BRS_FRAME_MUTEX);
        }

    }

    return received;
}

ConnectionStatusType C_BRSH_IO_Serial::connect()
{
    debugLog->BRS_Log() << "Attempting to Connect to BRSH..." << endl;

    if (openSerialPort())
    {
        debugLog->BRS_Log() << "Connected to BRSH!" << endl;
        connectionStatus = CONNECTED;
    }
    else
    {
        debugLog->BCI_Log() << "Could Not Connect to BRSH:(" << endl;
        connectionStatus = NOT_CONNECTED;
    }

    return connectionStatus;
}
