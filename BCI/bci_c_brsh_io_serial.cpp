#include "bci_c_brsh_io_serial.h"
#include "bci_c_binary_parser.h"
#include "bci_c_portnames.h"
#include <iostream>
using namespace std;

C_BRSH_IO_Serial::C_BRSH_IO_Serial()
{
    C_PortNames* portNames = C_PortNames::Instance();
    QString portName = portNames->GetBRSPortName();

    //Create the Serial Comm Class
    mSerialPortPtr = new C_Serial_Comm(portName, BAUD115200);
}

C_BRSH_IO_Serial::~C_BRSH_IO_Serial()
{

}

void printTMFrame(const TM_Frame_t* frame, std::ostream& stream)
{
	const char* delimeter = " ";

	//Write all the Data to the text File
	using namespace FILE_FORMAT;
	stream << (float) frame->timeStamp / 1000.0 << delimeter;
	stream << Int2String(BCI_STATES, BCI_STATES_SIZE, frame->bciState).toStdString()  << delimeter;
	stream << static_cast<char>(frame->lastCommand)     << delimeter;
	stream << Int2String(CONFIDENCES,CONFIDENCES_SIZE, frame->lastConfidence).toStdString() << delimeter;
	stream << static_cast<char>(frame->processingResult.command)           << delimeter;
	stream << Int2String(CONFIDENCES,CONFIDENCES_SIZE,frame->processingResult.confidence).toStdString() << delimeter;

	stream << frame->brsFrame.sensorData.gpsData.altitude     << delimeter;
	stream << frame->brsFrame.sensorData.gpsData.longitude    << delimeter;
	stream << frame->brsFrame.sensorData.gpsData.altitude     << delimeter;
	stream << frame->brsFrame.sensorData.gpsData.groundSpeed  << delimeter;
	stream << frame->brsFrame.sensorData.rangeFinderData.rangeFront << delimeter;
	stream << frame->brsFrame.sensorData.rangeFinderData.rangeBack  << delimeter;
	stream << (char) frame->brsFrame.remoteCommand << delimeter;

	stream << frame->ledForward .frequency   << delimeter;
	stream << frame->ledBackward.frequency   << delimeter;
	stream << frame->ledRight   .frequency   << delimeter;
	stream << frame->ledLeft    .frequency   << delimeter;
	stream << Int2String(CONN_STATUSES, CONN_STATUSES_SIZE, frame->eegConnectionStatus).toStdString()     << delimeter;
	stream << Int2String(CONN_STATUSES, CONN_STATUSES_SIZE, frame->pccConnectionStatus).toStdString()     << delimeter;
	stream << Int2String(CONN_STATUSES, CONN_STATUSES_SIZE, frame->brsConnectionStatus).toStdString()     << delimeter;
	stream << Int2String(CONN_STATUSES, CONN_STATUSES_SIZE, frame->flasherConnectionStatus).toStdString() << delimeter;

	stream << endl;
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
        mSerialPortPtr->sendToSerialPort(QByteArray("END"));
        mSerialPortPtr->sendToSerialPort(C_BinaryParser::TM_FRAME_START);
        debugLog->println(BRS_LOG, "Sending TM Frame");

        //printTMFrame(reinterpret_cast<const TM_Frame_t*>(&frame), cerr);
    }
}

bool C_BRSH_IO_Serial::fetchBRSFrame()
{
    bool         received       = false;
    uint32_t     bytesAvailable = mSerialPortPtr->bytesAvailable();
    uint32_t    index = 0;
    char        toPrint[50];
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

ConnectionStatusType C_BRSH_IO_Serial::GetConnectionStatus()
{
   connectionStatus = mSerialPortPtr->Connected();
   return connectionStatus;
}
