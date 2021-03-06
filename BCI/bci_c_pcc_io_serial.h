#ifndef BCI_C_JA2PCC
#define BCI_C_JA2PCC

#include "../CCS_PROJECTS/PCC/power_chair_command_constants.h"
#include "bci_c_serial_comm.h"
#include "bci_c_pcc_io.h"

class C_PCC_IO_Serial : public C_PCC_IO,
                        public C_Singleton<C_PCC_IO_Serial>
{
    Q_OBJECT
public:
     C_PCC_IO_Serial();
    ~C_PCC_IO_Serial();
	
    //Implementation of virtual functions
    virtual ConnectionStatusType connect();

public slots:
    virtual void SendCommand(PCC_Command_Type cmd);

    //Avoid All the Nonsense and send a quick E-Stop
    virtual void EmergencyStop();

private:
    C_Serial_Comm* mSerialPortPtr;
};

#endif // BCI_C_JA2PCC

