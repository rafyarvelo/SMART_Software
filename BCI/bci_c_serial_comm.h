#ifndef BCI_C_SERIAL_COMM_H
#define BCI_C_SERIAL_COMM_H

#include "../smart_config.h"
#include "../smart_data_types.h"
#include "../smart_debug_log.h"
#include "bci_c_portnames.h"
#include <QFile>
#include <QTextStream>
#include <QDataStream>

//Serial Comm Implementation using qextserialport
#include "qextserialport.h"

#define SUCCESS true
#define FAILURE false
#define DEFAULT_TIMEOUT_MS 500

#define READ_DATA_FILENAME ""

//Allocate enough space for two frames
#define MAX_BYTES_AVAILABLE (sizeof(TM_Frame_t) * 2 + 1)

//Wrapper class for serial communications
class C_Serial_Comm : public QObject
{
    Q_OBJECT//Required QT Macro
public:
    C_Serial_Comm(const QString& portName       , BaudRateType baudRate = BAUD9600,
                  FlowType flowCtrl = FLOW_OFF  , ParityType   parity   = PAR_NONE,
                  DataBitsType dataBits = DATA_8, StopBitsType stopBits = STOP_1,
                  long timeout = DEFAULT_TIMEOUT_MS);
    ~C_Serial_Comm();

    //return true on successful Open/Send
    bool openSerialPort();
    bool sendRawData(const char* pData, sizeType size);

    //Overloaded send functions
    bool sendToSerialPort(const QByteArray& bytes);
    bool sendToSerialPort(const unsigned char&   byte);
    bool sendToSerialPort(const unsigned short&  number);
    bool sendToSerialPort(const unsigned int&    number);
    bool sendToSerialPort(const TM_Frame_t*      pFrame);
    bool sendToSerialPort(const BRS_Frame_t*     pFrame);

    //Receive Data, return numBytes read or ByteArray with Data
    int         readRawData(char* pData, sizeType size);
    QByteArray& readFromSerialPort(int numBytes=-1);//numBytes < 0 to read all

    //Overloaded read functions
    int readFromSerialPort(unsigned char*  pByte);
    int readFromSerialPort(unsigned short* pShort);
    int readFromSerialPort(unsigned int*   pWord);
    int readFromSerialPort(TM_Frame_t*  pFrame);
    int readFromSerialPort(BRS_Frame_t* pFrame);

    //Return Connection Status
    bool Connected() { return mSerialPortPtr->isOpen(); }

    //Allow users to change the port settings dynamically
    void SetPortSettings(BaudRateType baudRate = BAUD9600,FlowType flowCtrl = FLOW_OFF,
                         ParityType   parity   = PAR_NONE,DataBitsType dataBits = DATA_8,
                         StopBitsType stopBits = STOP_1,long timeout = DEFAULT_TIMEOUT_MS);

    uint64t bytesAvailable(){ return mSerialPortPtr->bytesAvailable(); }

signals:
    void dataReceived(const QByteArray& data);
    void PortDisconnected();

private slots:
    void onDsrChanged(bool status);

protected:
    void printPortSettings(ostream& stream);
    SMART_DEBUG_LOG* debugLogPtr;

    QString         mPortName;
    QByteArray      UARTReceiveBuffer;

private:
    QextSerialPort* mSerialPortPtr;
};

#endif

