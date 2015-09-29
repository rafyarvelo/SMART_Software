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
    sizeType frameSize = (sizeType) sizeof(TM_Frame_t);
    unsigned int msgID = BCI2BRS_MSG_ID;

    //Write the TM Frame to the BRS through the UART Serial Port
    mSerialCommPtr->send(msgID);
    mSerialCommPtr->send(frameSize);
    mSerialCommPtr->sendRawData(reinterpret_cast<char*>(pFrame), frameSize);
}

bool C_BRSH_IO_Serial::fetchBRSFrame()
{
    bool received = false;
    u_int32_t msgID   = 0;
    sizeType  msgSize = 0;

    //Read message ID
    mSerialCommPtr->readRawData(reinterpret_cast<char*>(&msgID), sizeof(u_int32_t));

    //Try to read BRS Frame
    if (msgID == BRS2BCI_MSG_ID)
    {
        //Read size first
        mSerialCommPtr->readRawData(reinterpret_cast<char*>(&msgSize), sizeof(sizeType));
        mSerialCommPtr->readRawData(reinterpret_cast<char*>(&currentBRSFrame), sizeof(BRS_Frame_t));

        //Make sure we know what we're reading
        if (msgSize != sizeof(BRS_Frame_t))
        {
            QString msg = QString("\tmsgSize = ") + QString::number(msgSize) +
                          QString(", sizeof(BRS_Frame_t) = ") + QString::number(sizeof(BRS_Frame_t));
            debugLog->println(BRS_LOG, "Warning! BRS Frame size differs from Msg Size!");
            debugLog->println(BRS_LOG, msg.toStdString());
        }

        //Message receiving was successful
        else
        {
            received = true;
            emit BRSFrameReceived(&currentBRSFrame);
        }
    }

    return received;
}

ConnectionStatusType C_BRSH_IO_Serial::connect()
{
    debugLog->BRS_Log() << "Attempting to Connect to BRSH..." << endl;

    if (mSerialCommPtr->open())
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
