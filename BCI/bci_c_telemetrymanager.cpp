#include "bci_c_telemetrymanager.h"

//Mutual Exclusion to our TM Frame
QSemaphore* C_TelemetryManager::pTMFrameMutex  = new QSemaphore(TM_FRAME_MUTEX);

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

    //Initialize Current Frame to NULL
    pLatestTMFrame = 0;

    //Send a Frame every time it's Ready
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
    //Lock the TM Frame
    pTMFrameMutex->acquire(TM_FRAME_MUTEX);

    if (!pLatestTMFrame)
    {
        pLatestTMFrame = createTMFrame();
    }

    //Update TM from Interfaces
    pLatestTMFrame->timeStamp = mBCIPackagePtr->stopwatch.elapsed();

    //Make sure the BRS Frame is not Busy
    C_BRSH_IO::pBRSFrameMutex->acquire(BRS_FRAME_MUTEX);
    memcpy(&pLatestTMFrame->brsFrame, mBRS_IOPtr->GetLatestBRSFramePtr(), sizeof(BRS_Frame_t));
    C_BRSH_IO::pBRSFrameMutex->release(BRS_FRAME_MUTEX);

    memcpy(&pLatestTMFrame->eegFrame, mEEG_IOPtr->GetFramePtr(), sizeof(EEG_Frame_t));
    pLatestTMFrame->ledForward.frequency    = mRVSPtr->GetLEDGroup(LED_FORWARD) ->frequency;
    pLatestTMFrame->ledBackward.frequency   = mRVSPtr->GetLEDGroup(LED_BACKWARD)->frequency;
    pLatestTMFrame->ledRight.frequency      = mRVSPtr->GetLEDGroup(LED_RIGHT)   ->frequency;
    pLatestTMFrame->ledLeft.frequency       = mRVSPtr->GetLEDGroup(LED_LEFT)    ->frequency;
    pLatestTMFrame->eegConnectionStatus     = mBCIPackagePtr->eegConnectionStatus;
    pLatestTMFrame->brsConnectionStatus     = mBCIPackagePtr->brshConnectionStatus;
    pLatestTMFrame->pccConnectionStatus     = mBCIPackagePtr->pccConnectionStatus;
    pLatestTMFrame->flasherConnectionStatus = mBCIPackagePtr->flasherConnectionStatus;

    //Record TM to Output Files if requested
    if (recordTM)
    {
        OutputFrameToFile(pLatestTMFrame);
    }

    //Notify Listeners that our frame is ready
    emit tmFrameCreated(pLatestTMFrame);

    //Unlock the TM Frame
    pTMFrameMutex->release(TM_FRAME_MUTEX);

    return pLatestTMFrame;
}

TM_Frame_t* C_TelemetryManager::GetLatestFramePtr()
{
    if (!pLatestTMFrame)
    {
        updateTM();
    }

    return pLatestTMFrame;
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
