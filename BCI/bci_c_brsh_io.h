#ifndef C_BRSH_IO_H
#define C_BRSH_IO_H

#include <QThread>
#include <QTimer>
#include "../smart_config.h"
#include "../smart_debug_log.h"
#include "bci_c_singleton.h"
#include "bci_c_connected_device.h"
#include "bci_c_tm.h"
#include "bci_c_framegenerator.h"

//Abstract BRS IO Class, essentially just a worker-object
class C_BRSH_IO : public QObject, public C_ConnectedDevice
{
    Q_OBJECT
public:
     C_BRSH_IO();
     virtual ~C_BRSH_IO();

    //Return the latest BRS Data
    BRS_Frame_t& GetLatestBRSFrame()    { return  currentBRSFrame; }
    BRS_Frame_t* GetLatestBRSFramePtr() { return &currentBRSFrame; }

    //Connect to BRSH
    virtual ConnectionStatusType connect() = 0;

    //The Rate we will execute the BRSH IO Task (10 Hz)
    static const u_int16_t EXECUTION_RATE;

public slots:
    //Start Getting Data
    void start();

    //Try to Retrieve a Frame, return true and emit BRSFrameReceived() if successful
    virtual bool fetchBRSFrame() = 0;

    //Send a Frame to the BRSH
    virtual void SendTMFrame(TM_Frame_t* pFrame) = 0;


signals:
    void BRSFrameReceived(BRS_Frame_t* brsFrame);
    void remoteCommandReceived(PCC_Command_Type& cmd);
    void RequestEmergencyStop();

protected:
    SMART_DEBUG_LOG* debugLog;
    BRS_Frame_t      currentBRSFrame;
    QTimer           timer;
};

//Class to perform actual thread execution
class C_BRSH_IO_Task : public QThread
{
public:
    static C_BRSH_IO_Task* Instance(C_BRSH_IO* ptr);

    void run();

private://Private Constructor, Use Singleton Method
    C_BRSH_IO_Task(C_BRSH_IO* pBRSH_IO);

private:
    C_BRSH_IO* mBRSH_IO_Ptr;
    QTimer     timer;
};

#endif // C_BRSH_IO_H
