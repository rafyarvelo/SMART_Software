#ifndef C_BRSH_IO_H
#define C_BRSH_IO_H

#include <QThread>
#include "../smart_config.h"
#include "../smart_debug_log.h"
#include "bci_c_singleton.h"
#include "bci_c_connected_device.h"
#include "bci_c_tm.h"

//Abstract BRS IO Class
class C_BRSH_IO : public QThread, public C_ConnectedDevice
{
    Q_OBJECT
public:
     C_BRSH_IO(){}
    virtual ~C_BRSH_IO(){}

    //Send TM to the BRSH
    virtual void SendTMFrame(TM_Frame_t* pFrame) = 0;

    //Return the latest BRS Data
    virtual BRS_Frame_t* GetLatestBRSFrame() = 0;

    virtual ConnectionStatusType connect() = 0;

signals:
    void BRSFrameReceived(BRS_Frame_t& brsFrame);
    void remoteCommandReceived(PCC_Command_Type& cmd);
    void EmergencyStopRequested();

private:
    BRS_Frame_t currentBRSFrame;
};

#endif // C_BRSH_IO_H
