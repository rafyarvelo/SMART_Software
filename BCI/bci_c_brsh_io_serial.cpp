#include "bci_c_brsh_io_serial.h"

C_BRSH_IO_Serial::C_BRSH_IO_Serial()
    : C_Serial_Comm(BRS_PORT)
{
    //Tiva C Uses a different Baud Rate
    mSerialPortPtr->setBaudRate(BAUD115200);
}

C_BRSH_IO_Serial::~C_BRSH_IO_Serial()
{

}

//Send TM to the BRSH
void C_BRSH_IO_Serial::SendTMFrame(TM_Frame_t* pFrame)
{
    //Write the TM Frame to the BRS through the UART Serial Port
    sendToSerialPort(reinterpret_cast<const TM_Frame_t*>(pFrame));
}

bool C_BRSH_IO_Serial::fetchBRSFrame()
{
    bool       received       = false;
    u_int64_t  bytesAvailable = mSerialPortPtr->bytesAvailable();
    static int retryCount     = 0;
    uint8_t    i              = 0;

    //Message ID Buffer
    unsigned char msgId[MSG_ID_SIZE];

    //Check if we're Connected
    if (!mSerialPortPtr->isOpen())
    {
        if (++retryCount > MAX_MISS_COUNT)
        {
            connectionStatus = false;
            mSerialPortPtr->close();
        }
        else
        {
            return false; //Don't Waste time just wait until the next fetch
        }
    }

    //Don't Bother if the data isn't there yet
    if (bytesAvailable < (sizeof(BRS_Frame_t)))
    {
        return false;
    }

    //Flush the Port if we're getting full
    else if (bytesAvailable >= MAX_BYTES_AVAILABLE)
    {
        mSerialPortPtr->readAll();
    }

    //Allocate the Frame if it hasn't been done already
    if (!pLatestBRSFrame)
    {
        pLatestBRSFrame = createBRSFrame();
    }

    //Look for the Message ID one byte at a time
    while (bytesAvailable-- >= sizeof(BRS_Frame_t))
    {
        //Overwrite one byte in our buffer
        readFromSerialPort(&msgId[i++ % MSG_ID_SIZE]);

        //Try to read BRS Frame
        if (checkMsgID(reinterpret_cast<MSG_ID_Type>(msgId), BRS2BCI_MSG_ID))
        {
            debugLog->println(BRS_LOG, "MSG ID Received: BRS2BCI MSG ID\n");

            //Read in the Frame
            pBRSFrameMutex->acquire(BRS_FRAME_MUTEX);
            readFromSerialPort(pLatestBRSFrame);
            pBRSFrameMutex->release(BRS_FRAME_MUTEX);

            //Notify that it was received
            received = true;
            emit BRSFrameReceived(pLatestBRSFrame);

            //Wipe the Message ID Buffer
            memset(&msgId[0], 0, MSG_ID_SIZE);
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
