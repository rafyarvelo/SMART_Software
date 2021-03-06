#include "bci_c_serial_comm.h"

C_Serial_Comm::C_Serial_Comm(const QString& portName, BaudRateType baudRate, FlowType flowCtrl,
                             ParityType parity      , DataBitsType dataBits, StopBitsType stopBits,
                             long timeout)
    : mPortName(portName)
{
    debugLogPtr = SMART_DEBUG_LOG::Instance();

    //Open Serial Port and Set Port Settings
    mSerialPortPtr = new QextSerialPort(mPortName);
    SetPortSettings(baudRate,flowCtrl,parity, dataBits, stopBits, timeout);

    connect(mSerialPortPtr, SIGNAL(dsrChanged(bool)), this, SLOT(onDsrChanged(bool)));
}

C_Serial_Comm::~C_Serial_Comm()
{
    if (mSerialPortPtr)
    {
        mSerialPortPtr->close();
        delete mSerialPortPtr;
    }
}

void C_Serial_Comm::SetPortSettings(BaudRateType baudRate, FlowType flowCtrl    , ParityType parity,
                                    DataBitsType dataBits, StopBitsType stopBits, long timeout)
{
    //Configure Port Setup
    mSerialPortPtr->setBaudRate   (baudRate);
    mSerialPortPtr->setFlowControl(flowCtrl);
    mSerialPortPtr->setParity     (parity);
    mSerialPortPtr->setDataBits   (dataBits);
    mSerialPortPtr->setStopBits   (stopBits);
    mSerialPortPtr->setTimeout    (timeout);
}

void C_Serial_Comm::printPortSettings(ostream &stream)
{
    stream << "Port Settings for " << mPortName.toStdString() << ":" << endl;
    stream << "BAUD Rate:    " << mSerialPortPtr->baudRate()         << endl;
    stream << "Flow Control: " << mSerialPortPtr->flowControl()      << endl;
    stream << "Parity:       " << mSerialPortPtr->parity()           << endl;
    stream << "Data Bits:    " << mSerialPortPtr->dataBits()         << endl;
    stream << "Stop Bits:    " << mSerialPortPtr->stopBits()         << endl;
    stream << "Timeout (ms): " << DEFAULT_TIMEOUT_MS                 << endl;
}

bool C_Serial_Comm::openSerialPort()
{
    bool status;
    std::string toPrint;

    if (mSerialPortPtr->open(QIODevice::ReadWrite))
    {
        toPrint = std::string("Successfully Opened Port: ") + mPortName.toStdString();
        status = SUCCESS;
    }
    else
    {
        toPrint = std::string("Could Not Open Port: ") + mPortName.toStdString();
        status = FAILURE;
    }

    //Print Port Opening Status
    debugLogPtr->println(SERIAL_COMM_LOG, toPrint, true);
    printPortSettings(debugLogPtr->SerialComm_Log());
	
    return status;
}

bool C_Serial_Comm::sendRawData(const char* pData, sizeType size)
{
    int bytesWritten = -1;

    debugLogPtr->SerialComm_Log() << "Trying to Write Bytes: " << size << endl;
    bytesWritten = mSerialPortPtr->write(pData, (qint64) size);
    debugLogPtr->SerialComm_Log() << "Bytes Written: " << bytesWritten << endl;

    return (bytesWritten > -1) ? SUCCESS : FAILURE;
}

//Overloaded send functions

bool C_Serial_Comm::sendToSerialPort(const QByteArray& bytes)
{
    return sendRawData(bytes.data(), bytes.size());
}

bool C_Serial_Comm::sendToSerialPort(const unsigned char& byte)
{
    return sendRawData(reinterpret_cast<const char*>(&byte), sizeof(unsigned char));
}

bool C_Serial_Comm::sendToSerialPort(const unsigned short& number)
{
    return sendRawData(reinterpret_cast<const char*>(&number), sizeof(unsigned short));
}

bool C_Serial_Comm::sendToSerialPort(const unsigned int& number)
{
    return sendRawData(reinterpret_cast<const char*>(&number), sizeof(unsigned int));
}

bool C_Serial_Comm::sendToSerialPort(const TM_Frame_t* pFrame)
{
    return sendRawData(reinterpret_cast<const char*>(pFrame), sizeof(TM_Frame_t));
}

bool C_Serial_Comm::sendToSerialPort(const BRS_Frame_t* pFrame)
{
    return sendRawData(reinterpret_cast<const char*>(pFrame), sizeof(BRS_Frame_t));
}

//Receive Data and return bytes read
int C_Serial_Comm::readRawData(char* pData, sizeType size)
{
    int bytesAvailable = mSerialPortPtr->bytesAvailable();
    int bytesRead = -1;

    if (!pData)//Seriously bro...allocate your shit..
    {
        debugLogPtr->SerialComm_Log() << "Unallocated Buffer Passed to Read! Nothing will be read." << endl;
        return bytesRead;
    }

    //Only read how much we can
    if (size > bytesAvailable)
    {
        debugLogPtr->SerialComm_Log() << "Request for " << size << " Bytes, Available: " << bytesAvailable << endl;
        size = bytesAvailable;
    }

    //Read Data
    debugLogPtr->SerialComm_Log() << "Bytes Available: " << bytesAvailable << endl;
    bytesRead = mSerialPortPtr->read(pData, size);
    debugLogPtr->SerialComm_Log() << "Bytes Read: "      << bytesRead << endl;

    //See what we read
    debugLogPtr->SerialComm_Log() << "START: ";
    for (int i = 0; i < bytesRead/4; i++)
    {
        debugLogPtr->SerialComm_Log() << "0x" << hex << *reinterpret_cast<uint32_t*>(pData) << dec << " ";
    }
    debugLogPtr->SerialComm_Log() << " :STOP" << endl;

    return bytesRead;
}

QByteArray& C_Serial_Comm::readFromSerialPort(int numBytes)
{
    if (numBytes < 0 || numBytes > mSerialPortPtr->bytesAvailable())
    {
        UARTReceiveBuffer = mSerialPortPtr->readAll();
    }
    else
    {
        UARTReceiveBuffer = mSerialPortPtr->read(numBytes);
    }

    return UARTReceiveBuffer;
}

int C_Serial_Comm::readFromSerialPort(unsigned char* pByte)
{
    return readRawData(reinterpret_cast<char*>(pByte), sizeof(char));
}

int C_Serial_Comm::readFromSerialPort(unsigned short* pShort)
{
    return readRawData(reinterpret_cast<char*>(pShort), sizeof(unsigned short));
}


int C_Serial_Comm::readFromSerialPort(unsigned int* pWord)
{
    return readRawData(reinterpret_cast<char*>(pWord), sizeof(unsigned int));
}


int C_Serial_Comm::readFromSerialPort(TM_Frame_t*  pFrame)
{
    return readRawData(reinterpret_cast<char*>(pFrame), sizeof(TM_Frame_t));
}

int C_Serial_Comm::readFromSerialPort(BRS_Frame_t* pFrame)
{
    return readRawData(reinterpret_cast<char*>(pFrame), sizeof(BRS_Frame_t));
}

//SLOTS
void C_Serial_Comm::onDsrChanged(bool status)
{
    std::string toPrint = mPortName.toStdString();

    if (status)
    {
        toPrint += std::string(" was turned on");
        debugLogPtr->println(SERIAL_COMM_LOG, toPrint);
    }
    else //We Lost Connection
    {
        toPrint += std::string(" was turned off");
        debugLogPtr->println(SERIAL_COMM_LOG,toPrint ,false,true);
        emit PortDisconnected();
    }
}
