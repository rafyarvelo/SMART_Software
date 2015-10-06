#include "bci_c_serial_comm.h"

C_Serial_Comm::C_Serial_Comm(const QString& portName)
    : mPortName(portName)
{
    debugLog    = SMART_DEBUG_LOG::Instance();//Get a pointer to the debug log
    mSerialPortPtr = new QextSerialPort(mPortName);

    //Config Port Setup
    SetDefaultPortSettings();

    connect(mSerialPortPtr, SIGNAL(dsrChanged(bool)), this, SLOT(onDsrChanged(bool)));
    connect(mSerialPortPtr, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

C_Serial_Comm::~C_Serial_Comm()
{
    if (mSerialPortPtr)
    {
        mSerialPortPtr->close();
        delete mSerialPortPtr;
    }
}

void C_Serial_Comm::SetDefaultPortSettings()
{
    //Config Port Setup
    mSerialPortPtr->setBaudRate   (BAUD9600);
    mSerialPortPtr->setFlowControl(FLOW_OFF);
    mSerialPortPtr->setParity     (PAR_NONE);
    mSerialPortPtr->setDataBits   (DATA_8);
    mSerialPortPtr->setStopBits   (STOP_1);
    mSerialPortPtr->setTimeout    (DEFAULT_TIMEOUT_MS);
}

void C_Serial_Comm::printPortSettings(ostream &stream)
{
    stream << "Port Settings for " << mPortName.toStdString() << ":" << endl;
    stream << "BAUD Rate:    " << mSerialPortPtr->baudRate()            << endl;
    stream << "Flow Control: " << mSerialPortPtr->flowControl()         << endl;
    stream << "Parity:       " << mSerialPortPtr->parity()              << endl;
    stream << "Data Bits:    " << mSerialPortPtr->dataBits()            << endl;
    stream << "Stop Bits:    " << mSerialPortPtr->stopBits()            << endl;
    stream << "Timeout (ms): " << DEFAULT_TIMEOUT_MS                 << endl;
}

bool C_Serial_Comm::openSerialPort()
{
    if (mSerialPortPtr->open(QIODevice::ReadWrite))
    {
        cout                << "Successfully Opened Port: " << mPortName.toStdString() << endl;
        debugLog->BCI_Log() << "Successfully Opened Port: " << mPortName.toStdString() << endl;

        printPortSettings(debugLog->SerialComm_Log());
    }
    else
    {
        cout                << "Could Not Open Port: " << mPortName.toStdString() << endl;
        debugLog->BCI_Log() << "Could Not Open Port: " << mPortName.toStdString() << endl;
        printPortSettings(debugLog->BCI_Log());
        return FAILURE;
    }
	
	return SUCCESS;
}

bool C_Serial_Comm::sendRawData(const char* pData, sizeType size)
{
    int bytesWritten = -1;

    debugLog->SerialComm_Log() << "Trying to Write Bytes: " << size << endl;
    bytesWritten = mSerialPortPtr->write(pData, (qint64) size);
    debugLog->SerialComm_Log() << "Bytes Written: " << bytesWritten << endl;

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
        debugLog->SerialComm_Log() << "Unallocated Buffer Passed to Read! Nothing will be read." << endl;
        return bytesRead;
    }

    //Only read how much we can
    if (size > bytesAvailable)
    {
        debugLog->SerialComm_Log() << "Request for " << size << " Bytes, Available: " << bytesAvailable << endl;
        size = bytesAvailable;
    }

    //Read Data
    debugLog->SerialComm_Log() << "Bytes Available: " << bytesAvailable << endl;
    bytesRead = mSerialPortPtr->read(pData, size);
    debugLog->SerialComm_Log() << "Bytes Read: "      << bytesRead << endl;

    //See what we read
    debugLog->SerialComm_Log() << "START: ";
    for (int i = 0; i < bytesRead/4; i++)
    {
        debugLog->SerialComm_Log() << "0x" << hex << *reinterpret_cast<u_int32_t*>(pData) << dec << " ";
    }
    debugLog->SerialComm_Log() << " :STOP" << endl;

    return bytesRead;
}

QByteArray C_Serial_Comm::readFromSerialPort(int numBytes)
{
    QByteArray bytes;
    int numToRead;

    if (numBytes < 0 || numBytes > mSerialPortPtr->bytesAvailable())
    {
        numToRead = mSerialPortPtr->bytesAvailable();
    }
    else
    {
        numToRead = numBytes;
    }

    bytes = mSerialPortPtr->read(numBytes);
    return bytes;
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
    if (status)
    {
        debugLog->SerialComm_Log() << "Serial Device was turned on" << endl;
    }
    else
    {
        debugLog->SerialComm_Log() << "Serial Device was turned off" << endl;
    }
}

void C_Serial_Comm::onReadyRead()
{
    //this->readAll();//Read all available data when it's ready
}
