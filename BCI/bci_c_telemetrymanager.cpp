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

    //Send a Frame every time it's Ready
    connect(this      , SIGNAL(tmFrameCreated(tmFrameBufferType*)),
            mBRS_IOPtr, SLOT(SendTMFrame(tmFrameBufferType*)));
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
const TM_Frame_t& C_TelemetryManager::updateTM(BRS_Frame_t& brsFrame)
{

    //Update TM from Interfaces
    mCurrentTMFrame.timeStamp        = mBCIPackagePtr->stopwatch.elapsed();
    mCurrentTMFrame.bciState         = mBCIPackagePtr->bciState;
    mCurrentTMFrame.lastCommand      = mJA_Ptr->finalCommand;
    mCurrentTMFrame.lastConfidence   = mJA_Ptr->cmdConfidence;

    //The Processing Result that the Judgment Algorithm is using
    mCurrentTMFrame.processingResult.command    = mJA_Ptr->mCurrentProcessingResult.command;
    mCurrentTMFrame.processingResult.confidence = mJA_Ptr->mCurrentProcessingResult.confidence;

    //Update BRS Frame
    memcpy(&mCurrentTMFrame.brsFrame, &brsFrame, sizeof(BRS_Frame_t));

    //Update the Rest of the Data
    mCurrentTMFrame.ledForward.frequency    = mRVSPtr->GetLEDGroup(LED_FORWARD) ->frequency;
    mCurrentTMFrame.ledBackward.frequency   = mRVSPtr->GetLEDGroup(LED_BACKWARD)->frequency;
    mCurrentTMFrame.ledRight.frequency      = mRVSPtr->GetLEDGroup(LED_RIGHT)   ->frequency;
    mCurrentTMFrame.ledLeft.frequency       = mRVSPtr->GetLEDGroup(LED_LEFT)    ->frequency;
    mCurrentTMFrame.eegConnectionStatus     = mBCIPackagePtr->eegConnectionStatus;
    mCurrentTMFrame.brsConnectionStatus     = mBCIPackagePtr->brshConnectionStatus;
    mCurrentTMFrame.pccConnectionStatus     = mBCIPackagePtr->pccConnectionStatus;
    mCurrentTMFrame.flasherConnectionStatus = mBCIPackagePtr->flasherConnectionStatus;

    //Record TM to Output Files if requested
    if (recordTM)
    {
        OutputFrameToFile(&mCurrentTMFrame);
    }

    //Notify Listeners that our frame is ready
    tmFrameBuffer.Put(mCurrentTMFrame);
    emit tmFrameCreated(&tmFrameBuffer);

    return mCurrentTMFrame;
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
