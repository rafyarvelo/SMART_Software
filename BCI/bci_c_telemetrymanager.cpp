#include "bci_c_telemetrymanager.h"

C_TelemetryManager::C_TelemetryManager(C_BCI_Package* pBCI, C_EEG_IO* pEEG_IO, C_BRSH_IO* pBRS_IO,
                                       C_RVS*         pRVS, C_JudgmentAlgorithm* pJA)
{
    debugLog = SMART_DEBUG_LOG::Instance();

    //Initialize Dependencies
    mBCIPackagePtr = pBCI;
    mEEG_IOPtr     = pEEG_IO;
    mBRS_IOPtr     = pBRS_IO;
    mRVSPtr        = pRVS;
    mJA_Ptr        = pJA;

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
C_TelemetryManager* C_TelemetryManager::Instance(C_BCI_Package* pBCI, C_EEG_IO* pEEG_IO, C_BRSH_IO* pBRS_IO,
                                                 C_RVS*         pRVS, C_JudgmentAlgorithm* pJA)
{
    return new C_TelemetryManager(pBCI, pEEG_IO, pBRS_IO, pRVS, pJA);
}


//Create a New TM Frame from the Latest Data
TM_Frame_t* C_TelemetryManager::updateTM(BRS_Frame_t* pBRSFrame)
{
    //Make default frames if pointers are invalid
    if (!pLatestTMFrame)
    {
        pLatestTMFrame = createTMFrame();
    }
    else if (!pBRSFrame)
    {
        pBRSFrame = createBRSFrame();
    }

    //Update TM from Interfaces
    pLatestTMFrame->timeStamp        = mBCIPackagePtr->stopwatch.elapsed();
    pLatestTMFrame->bciState         = mBCIPackagePtr->bciState;
    pLatestTMFrame->lastCommand      = mJA_Ptr->finalCommand;
    pLatestTMFrame->lastConfidence   = mJA_Ptr->cmdConfidence;

    //The Processing Result that the Judgment Algorithm is using
    pLatestTMFrame->processingResult.command    = mJA_Ptr->mCurrentProcessingResult.command;
    pLatestTMFrame->processingResult.confidence = mJA_Ptr->mCurrentProcessingResult.confidence;

    //Update BRS Frame
    memcpy(&pLatestTMFrame->brsFrame, pBRSFrame, sizeof(BRS_Frame_t));

    //Update the Rest of the Data
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

    return pLatestTMFrame;
}

TM_Frame_t* C_TelemetryManager::GetLatestFramePtr()
{
    //Don't return a null frame
    if (!pLatestTMFrame)
    {
        updateTM(createBRSFrame());
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
