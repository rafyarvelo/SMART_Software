#ifndef BCI_C_JA2PCC
#define BCI_C_JA2PCC

#include "../CCS_PROJECTS/PCC/power_chair_command_constants.h"
#include "bci_c_serial_comm.h"
#include "bci_c_safequeue.h"
#include "bci_c_pcc_io.h"
#include <QTimer>
#include <QThread>

#define PCC_COMMAND_BUFFER_SIZE 5
#define PCC_COMMAND_SEND_RATE   1000 //1 Hz

class C_PCC_IO_Serial : public C_PCC_IO,
                        public C_Serial_Comm,
                        public C_Singleton<C_PCC_IO_Serial>,
                        public QObject
{
    Q_OBJECT
public:
     C_PCC_IO_Serial();
    ~C_PCC_IO_Serial();
	
    //Implementation of virtual functions
    virtual ConnectionStatusType connect();

public slots:
    //Only we can decide when to send commands, so this just
    //Puts the Command in our Buffer
    virtual void SendCommand(PCC_Command_Type cmd);

     //Avoid All the Nonsense and send a quick E-Stop
    virtual void EmergencyStop();

private slots:
     void onTimer();

private:
     //Actually Send the Command through the UART on a Timer Event
     void SendCmdToSerialPort(PCC_Command_Type cmd);
    //We will need to buffer our PCC Commands so that
    //they don't get sent too fast. Only Actually Send the data on a Timer event
    C_SafeQueue<PCC_Command_Type>* pccCommandQueue;

    QTimer       mTimer;
    QThread      mThread;
};

#endif // BCI_C_JA2PCC

