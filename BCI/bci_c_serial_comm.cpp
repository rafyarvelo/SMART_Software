#include "bci_c_serial_comm.h"

C_Serial_Comm::C_Serial_Comm(const QString&      portName)
    : mPortName(portName)
{
    debugLog    = SMART_DEBUG_LOG::Instance();//Get a pointer to the debug log
    mSerialPort = new QextSerialPort(mPortName);

    //Config Port Setup
    SetDefaultPortSettings();

    connect(mSerialPort, SIGNAL(dsrChanged(bool)), this, SLOT(onDsrChanged(bool)));
    connect(mSerialPort, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

C_Serial_Comm::~C_Serial_Comm()
{
    if (mSerialPort)
    {
        mSerialPort->close();
        delete mSerialPort;
    }
}

void C_Serial_Comm::SetDefaultPortSettings()
{
    //Config Port Setup
    mSerialPort->setBaudRate(BAUD9600);
    mSerialPort->setFlowControl(FLOW_OFF);
    mSerialPort->setParity(PAR_NONE);
    mSerialPort->setDataBits(DATA_8);
    mSerialPort->setStopBits(STOP_1);
    mSerialPort->setTimeout(DEFAULT_TIMEOUT_MS);
}

void C_Serial_Comm::printPortSettings(ostream &stream)
{
    stream << "Port Settings for " << mPortName.toStdString() << ":" << endl;
    stream << "BAUD Rate:    " << mSerialPort->baudRate()            << endl;
    stream << "Flow Control: " << mSerialPort->flowControl()         << endl;
    stream << "Parity:       " << mSerialPort->parity()              << endl;
    stream << "Data Bits:    " << mSerialPort->dataBits()            << endl;
    stream << "Stop Bits:    " << mSerialPort->stopBits()            << endl;
    stream << "Timeout (ms): " << DEFAULT_TIMEOUT_MS                 << endl;
}

bool C_Serial_Comm::open()
{
    if (mSerialPort->open(QIODevice::ReadWrite))
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

C_Serial_Comm* C_Serial_Comm::Instance(const QString& portName)
{
    return new C_Serial_Comm(portName);
}

bool C_Serial_Comm::sendRawData(const char* pData, sizeType size)
{
    int bytesWritten = -1;

    debugLog->BCI_Log() << "Attempting to Write Data of size: " << size << endl;
    bytesWritten = mSerialPort->write(pData, (qint64) size);
    debugLog->BCI_Log() << "Bytes Written: " << bytesWritten << endl;

    read();//Hack for now...

    return (bytesWritten > -1) ? SUCCESS : FAILURE;
}

bool C_Serial_Comm::send(QByteArray& bytes)
{
    return sendRawData(bytes.data(), bytes.size());
}

//Receive Data
void C_Serial_Comm::readRawData(char* pData, sizeType size)
{
    int bytesAvailable = mSerialPort->bytesAvailable();

    if (!pData)//Seriously bro...allocate your shit..
    {
        debugLog->BCI_Log() << "Unallocated Buffer Passed to Read! Nothing will be read." << endl;
        return;
    }

    //Read Data
    debugLog->BCI_Log() << "Bytes Available: " << bytesAvailable << endl;
    mSerialPort->read(pData, size);
    debugLog->BCI_Log() << "Bytes Read: " << size << endl;

    //See what we read
    std::string s = QString(QByteArray(pData, size)).toStdString();
    debugLog->SerialComm_Log() << s << endl;
}

QByteArray C_Serial_Comm::read(int numBytes)
{
    QByteArray bytes;
    int numToRead;

    if (numBytes < 0 || numBytes > mSerialPort->bytesAvailable())
    {
        numToRead = mSerialPort->bytesAvailable();
    }
    else
    {
        numToRead = numBytes;
    }

    //bytes.resize(numToRead);
    //readRawData(bytes.data(), bytes.size());

    bytes = mSerialPort->readAll();
    return bytes;
}

//SLOTS
void C_Serial_Comm::onDsrChanged(bool status)
{
    if (status)
        debugLog->SerialComm_Log() << "Serial Device was turned on" << endl;
    else
        debugLog->SerialComm_Log() << "Serial Device was turned off" << endl;
}

void C_Serial_Comm::onReadyRead()
{
    this->read();//Read all available data when it's ready
}
