#ifndef C_BRSH_IO_H
#define C_BRSH_IO_H

#include "../smart_config.h"
#include "../smart_debug_log.h"
#include "bci_c_connected_device.h"
#include "bci_c_tm.h"

class C_BRSH_IO : public C_ConnectedDevice
{
public:
     C_BRSH_IO();
    ~C_BRSH_IO();

    static C_BRSH_IO* Instance(){ return new C_BRSH_IO; }

    //Send TM to the BRSH
    void SetTMFrame(TM_Frame_t* pFrame);
    void SendTMFrame();

    //Return the latest BRS Data
    BRS_Frame_t& GetLatestBRSFrame();


    virtual ConnectionStatusType connect() { return CONNECTED; }

signals:
    void BRSDataReceived(BRS_Frame_t& brsData);
    void remoteCommandReceived(PCC_Command_Type cmd);
    void EmergencyStopRequested();

private:
    TM_Frame_t  currentTMFrame;
    BRS_Frame_t currentBRSFrame;
};

#endif // C_BRSH_IO_H
