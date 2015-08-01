#ifndef BCI_C_SERIAL_COMM_H
#define BCI_C_SERIAL_COMM_H

#include "../smart_config.h"
#include "../smart_debug_log.h"

//Serial Comm Implementation using boost::asio
#include <boost/asio.hpp>
using namespace boost::asio;

//Default Serial Configuration
#define DEFAULT_BAUD_RATE 9600
#define DEFAULT_PARITY    serial_port_base::parity::none
#define DEFAULT_STOP_BITS serial_port_base::stop_bits::one

typedef boost::system::system_error SerialCommError;
#define SUCCESS true
#define FAILURE false

//Wrapper class for serial communications
class C_Serial_Comm
{
public://constructor with defaults
	 C_Serial_Comm(const char*                 portName,
	               serial_port_base::baud_rate baudRate =DEFAULT_BAUD_RATE,
	               serial_port_base::parity    parity   =DEFAULT_PARITY,
	               serial_port_base::stop_bits stopBits =DEFAULT_STOP_BITS);
	~C_Serial_Comm();
	
	//return true on successful Open/Write
	bool open();
	bool send(const char* pData, sizeType size);
	bool send(std::string data);
	
	//Receive Data
	void readRawData(char* pData, sizeType size);
	void readString (std::string& readBuffer);
	void readUntil  (std::string& readBuffer, char delim);
	void readLine   (std::string& readBuffer);
				
private:
	SMART_DEBUG_LOG* debugLog;
	
	const char*                 mPortName;
	serial_port_base::baud_rate mBaudRate;
	serial_port_base::parity    mParity;
	serial_port_base::stop_bits mStopBits;
	
	io_service  io;
	serial_port mSerialPort;
};

#endif

