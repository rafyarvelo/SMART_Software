#ifndef BCI_C_SERIAL_COMM_H
#define BCI_C_SERIAL_COMM_H

#include "../smart_config.h"
#include "../smart_debug_log.h"
#include <QFile>
#include <QTextStream>
#include <QDataStream>

//Serial Comm Implementation using qextserialport
#include "qextserialport.h"

#define SUCCESS true
#define FAILURE false
#define DEFAULT_TIMEOUT_MS 500

#define READ_DATA_FILENAME ""
//Wrapper class for serial communications
class C_Serial_Comm : public QObject
{
    Q_OBJECT//Required QT Macro
public:
     C_Serial_Comm(const QString&  portName);
	~C_Serial_Comm();

    static C_Serial_Comm* Instance(const QString& portName);

    //return true on successful Open/Send
    bool open();
    bool sendRawData(const char* pData, sizeType size);

    //Overloaded send functions
    bool send(QByteArray& bytes);
    bool send(unsigned char &  byte);
    bool send(unsigned short&  number);
    bool send(unsigned int  &  number);

	//Receive Data
    void       readRawData(char* pData, sizeType size);
    QByteArray read       (int numBytes=-1);//numBytes < 0 to read all

    void SetDefaultPortSettings();

private slots:
    void onDsrChanged(bool status);
    void onReadyRead();

private:
    void printPortSettings(ostream& stream);
    SMART_DEBUG_LOG* debugLog;

    QString         mPortName;
    QextSerialPort* mSerialPort;
};

#endif

