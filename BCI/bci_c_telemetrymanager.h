#ifndef BCI_C_TELEMETRYMANAGER_H
#define BCI_C_TELEMETRYMANAGER_H

//Forward Declation
class C_BCI_Package;

#include "bci_c_bci_package.h"
#include "bci_c_binary_parser.h"
#include "bci_c_textparser.h"

#define TM_FRAME_MUTEX 1 //Keeping only 1 Frame of TM (Latest) for Concurrent Access

//Manage the SMART Telemetry Stream
class C_TelemetryManager : public QObject
{
    Q_OBJECT
public:
    C_TelemetryManager(C_BCI_Package* pBCI, C_EEG_IO* pEEG_IO,
                       C_BRSH_IO* pBRS_IO,  C_RVS* pRVS);
    ~C_TelemetryManager();

    static C_TelemetryManager* Instance(C_BCI_Package* pBCI, C_EEG_IO* pEEG_IO,
                                        C_BRSH_IO* pBRS_IO , C_RVS* pRVS);

    //Create a New TM Frame from the Latest Data
    TM_Frame_t* updateTM();

    //Retrieve the Latest Frame
    TM_Frame_t* GetLatestFramePtr();

    //Record Telemetry to an output File
    void RecordTMToFile(const QString& filename);

    //A Mutex to Protect Concurrent Access to TM Frame
    static QSemaphore* pTMFrameMutex;

signals:
    void tmFrameCreated(TM_Frame_t* frame);

private:
    void OutputFrameToFile(TM_Frame_t* frame);

    C_BCI_Package* mBCIPackagePtr;
    C_EEG_IO*      mEEG_IOPtr;
    C_BRSH_IO*     mBRS_IOPtr;
    C_RVS*         mRVSPtr;

    //Used to Record TM to File
    C_BinaryParser* tmFile;

    //The Latest TM Frame
    TM_Frame_t*     pLatestTMFrame;

    bool recordTM;

    //Debug Logging
    SMART_DEBUG_LOG* debugLog;

public:

};

#endif // BCI_C_TELEMETRYMANAGER_H
