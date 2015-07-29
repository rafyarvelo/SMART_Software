#include "bci_c_serial_comm.h"

C_Serial_Comm::C_Serial_Comm(const char*                 portName,
                             serial_port_base::baud_rate baudRate,
                             serial_port_base::parity    parity,
                             serial_port_base::stop_bits stopBits)
    : mPortName(portName),
      mBaudRate(baudRate),
      mParity  (parity),
      mStopBits(stopBits),
      mSerialPort(io)
{
	debugLog = SMART_DEBUG_LOG::Instance();//Get a pointer to the debug log
}

C_Serial_Comm::~C_Serial_Comm()
{
	mSerialPort.close();
}

bool C_Serial_Comm::open()
{
	try
	{   //Attempt to open and configure serial port
		mSerialPort.open(mPortName);
		mSerialPort.set_option(mBaudRate);
		mSerialPort.set_option(mParity);
		mSerialPort.set_option(mStopBits);
	}
	catch (SerialCommError e)
	{
		debugLog->BCI_Log() << "Could not open serial port: " << mPortName << endl;
		return FAILURE;
	}
	
	return SUCCESS;
}

bool C_Serial_Comm::send(const char* pData, sizeType size)
{
	try
	{   //Attempt to open and configure serial port
		write(mSerialPort, buffer(pData, size));
	}
	catch (SerialCommError e)
	{
		debugLog->BCI_Log() << "Could not write to serial port: " << mPortName << endl;
		return FAILURE;
	}
	
	return SUCCESS;
}

bool C_Serial_Comm::send(std::string data)
{
	return send(data.c_str(), data.size());
}

//Receive Data
void C_Serial_Comm::readRawData(char* pData, sizeType size)
{
	
}

void C_Serial_Comm::readString(std::string& readBuffer)
{
	
}

void C_Serial_Comm::readUntil(std::string& readBuffer, char delim)
{
	
}

void C_Serial_Comm::readLine(std::string& readBuffer)
{
	return readUntil(readBuffer, '\n');
}
