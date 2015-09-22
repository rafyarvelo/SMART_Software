#include "bci_c_telemetrymanager.h"

C_TelemetryManager::C_TelemetryManager(C_BCI_Package* pBCI, C_EEG_IO* pEEG_IO,
                                       C_BRSH_IO* pBRS_IO,  C_RVS* pRVS)
{
    debugLog = SMART_DEBUG_LOG::Instance();

    //Initialize Dependencies
    mBCIPackagePtr = pBCI;
    mEEG_IOPtr     = pEEG_IO;
    mBRS_IOPtr     = pBRS_IO;
    mRVSPtr        = pRVS;

    //Don't Record TM By Default
    recordTM = false;

    //Make Connections
    connect(mBRS_IOPtr, SIGNAL(BRSFrameReceived(BRS_Frame_t*)),
            this      , SLOT(onBRSFrameReceived(BRS_Frame_t*)));

    connect(this      , SIGNAL(tmFrameCreated(TM_Frame_t*)),
            mBRS_IOPtr, SLOT(SendTMFrame(TM_Frame_t*)));
}

C_TelemetryManager::~C_TelemetryManager()
{
}

//Factory Constructor
C_TelemetryManager* C_TelemetryManager::Instance(C_BCI_Package* pBCI, C_EEG_IO* pEEG_IO,
                                                 C_BRSH_IO* pBRS_IO, C_RVS* pRVS)
{
    return new C_TelemetryManager(pBCI, pEEG_IO, pBRS_IO, pRVS);
}


//Create a New TM Frame from the Latest Data
TM_Frame_t* C_TelemetryManager::updateTM()
{
    TM_Frame_t* frame = TM_Frame_t::createFrame();

    //Update TM from Interfaces
    frame->timeStamp               = mBCIPackagePtr->stopwatch.elapsed();
    frame->brsFrame                = mBRS_IOPtr    ->GetLatestBRSFrame();
    frame->eegFrame                = mEEG_IOPtr    ->GetFramePtr();
    frame->ledForward.frequency    = mRVSPtr->GetLEDGroup(LED_FORWARD) ->frequency;
    frame->ledBackward.frequency   = mRVSPtr->GetLEDGroup(LED_BACKWARD)->frequency;
    frame->ledRight.frequency      = mRVSPtr->GetLEDGroup(LED_RIGHT)   ->frequency;
    frame->ledLeft.frequency       = mRVSPtr->GetLEDGroup(LED_LEFT)    ->frequency;
    frame->eegConnectionStatus     = mBCIPackagePtr->eegConnectionStatus;
    frame->brsConnectionStatus     = mBCIPackagePtr->brshConnectionStatus;
    frame->pccConnectionStatus     = mBCIPackagePtr->pccConnectionStatus;
    frame->flasherConnectionStatus = mBCIPackagePtr->flasherConnectionStatus;

    //Record TM to Output Files if requested
    if (recordTM)
    {
        OutputFrameToFile(frame);
    }

    //Notify Listeners that our frame is ready
    emit tmFrameCreated(frame);

    //Buffer and Return Frame
    mTMData.addFrame(frame);
    return frame;
}

void C_TelemetryManager::OutputFrameToFile(TM_Frame_t* frame)
{
    tmFile->writeTMFrame(frame);
}

//Record Telemetry to an output File
void C_TelemetryManager::RecordTMToFile(const QString& filename)
{
    string temp = string("Recording Telemetry to File: ") + filename.toStdString();
    debugLog->println(BCI_LOG, temp, true);

    recordTM = true;
    tmFile   = new C_BinaryParser(filename, QIODevice::WriteOnly);
}
