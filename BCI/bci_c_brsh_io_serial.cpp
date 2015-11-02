#include "bci_c_brsh_io_serial.h"

C_BRSH_IO_Serial::C_BRSH_IO_Serial()
{
    //Create the Serial Comm Class
    mSerialPortPtr = new C_Serial_Comm(BRS_PORT, BAUD115200);
}

C_BRSH_IO_Serial::~C_BRSH_IO_Serial()
{

}

//Send TM to the BRSH
void C_BRSH_IO_Serial::SendTMFrame(tmFrameBufferType* pBuffer)
{
    TM_Frame_t frame;
    bool status = false;

    frame = pBuffer->Get(&status);

    if (status)
    {
        //Write the TM Frame to the BRS through the UART Serial Port
        mSerialPortPtr->sendToSerialPort(reinterpret_cast<const TM_Frame_t*>(&frame));
        debugLog->println(BRS_LOG, "Sending TM Frame");
    }
}

bool C_BRSH_IO_Serial::fetchBRSFrame()
{
    bool         received       = false;
    uint32_t     bytesAvailable = mSerialPortPtr->bytesAvailable();
    uint32_t    index = 0;
    char        toPrint[25];
    QByteArray buffer;

    //Don't Bother if the data isn't there yet
    if (bytesAvailable < (sizeof(BRS_Frame_t)))
    {
        return false;
    }

    //Read in all the bytes from the Serial Port
    buffer = mSerialPortPtr->readFromSerialPort();

    //Send out all the BRS Frames we find
    while (buffer.contains(BRS2BCI_MSG_ID))
    {
        //Copy the Byte Array Contents To a BRS Frame and store it in the buffer
        index = buffer.indexOf(BRS2BCI_MSG_ID);
        memcpy(&mCurrentBRSFrame, buffer.data() + index, sizeof(BRS_Frame_t));
        brsFrameBuffer.Put(mCurrentBRSFrame);

        //Status Print
        sprintf(toPrint, "BRS Message Received: Command = %c", mCurrentBRSFrame.remoteCommand);
        debugLog->println(BRS_LOG,toPrint , true);

        //Notify that a frame is now in the buffer
        received = true;
        emit BRSFrameReceived(&brsFrameBuffer);

        //Move the Buffer over the Msg Id before we start again
        buffer = QByteArray::fromRawData(buffer.data() + index + sizeof(MsgIdType), buffer.size() - index - sizeof(MsgIdType));
    }

    return received;
}

ConnectionStatusType C_BRSH_IO_Serial::connect()
{
    debugLog->BRS_Log() << "Attempting to Connect to BRSH..." << endl;

    if (mSerialPortPtr->openSerialPort())
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

void C_BRSH_IO_Serial::onPortDisconnected()
{
    connectionStatus = NOT_CONNECTED;
}
