#ifndef BCI_C_TELEMETRYMANAGER_H
#define BCI_C_TELEMETRYMANAGER_H

//Forward Declation
class C_BCI_Package;

#include "bci_c_bci_package.h"
#include "bci_c_pcc_io.h"
#include "bci_c_binary_parser.h"
#include "bci_c_textparser.h"
#include "bci_c_judgment_algorithm.h"
#include "bci_c_safequeue.h"

//Forward Declaration of Telemetry Manager Class
class C_JudgmentAlgorithm;

//Manage the SMART Telemetry Stream
class C_TelemetryManager : public QObject
{
    Q_OBJECT
public:
    C_TelemetryManager(C_BCI_Package* pBCI, C_EEG_IO* pEEG_IO, C_BRSH_IO* pBRS_IO,
                       C_RVS*         pRVS, C_PCC_IO* pPCC_IO, C_JudgmentAlgorithm* pJA);
    ~C_TelemetryManager();

    static C_TelemetryManager* Instance(C_BCI_Package* pBCI, C_EEG_IO* pEEG_IO, C_BRSH_IO* pBRS_IO,
                                        C_RVS*         pRVS, C_PCC_IO* pPCC_IO, C_JudgmentAlgorithm* pJA);

    //Retrieve the Latest Frame
    const TM_Frame_t& GetLatestFrame() { return mCurrentTMFrame; }

    //Record Telemetry to an output File
    void RecordTMToFile(const QString& filename);

signals:
    void tmFrameCreated(tmFrameBufferType* tmFrameBuffer);

public slots:
    //Create a New TM Frame from the Latest Data
    const TM_Frame_t& updateTM(BRS_Frame_t& brsFrame);

private:
    void OutputFrameToFile(TM_Frame_t* frame);

    C_BCI_Package*       mBCIPackagePtr;
    C_EEG_IO*            mEEG_IOPtr;
    C_BRSH_IO*           mBRS_IOPtr;
    C_RVS*               mRVSPtr;
    C_PCC_IO*            mPCC_IOPtr;
    C_JudgmentAlgorithm* mJA_Ptr;

    //Used to Record TM to File
    C_BinaryParser* tmFile;

    //Store TM Frames in a Circular Buffer
    tmFrameBufferType tmFrameBuffer;
    TM_Frame_t        mCurrentTMFrame;

    //Log TM Flag
    bool recordTM;

    //Debug Logging
    SMART_DEBUG_LOG* debugLog;

public:

};

#endif // BCI_C_TELEMETRYMANAGER_H
