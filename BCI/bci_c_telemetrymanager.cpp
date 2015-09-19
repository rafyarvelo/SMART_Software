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

    outputBinaryFile = false;
    outputTextFile   = false;
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
    frame->eegFrame                = mEEG_IOPtr    ->getFrame();
    frame->ledGroups               = mRVSPtr       ->GetAllLEDGroups();
    frame->eegConnectionStatus     = mBCIPackagePtr->eegConnectionStatus;
    frame->brsConnectionStatus     = mBCIPackagePtr->brshConnectionStatus;
    frame->pccConnectionStatus     = mBCIPackagePtr->pccConnectionStatus;
    frame->flasherConnectionStatus = mBCIPackagePtr->flasherConnectionStatus;

    //Record TM to Output Files if requested
    if (outputBinaryFile)
    {
        OutputFrame(frame, BINARY_FILE);
    }

    if (outputTextFile)
    {
        OutputFrame(frame, TEXT_FILE);
    }

    return frame;
}

void C_TelemetryManager::OutputFrame(TM_Frame_t* frame, FileFormatType format)
{
    if (BINARY_FILE == format)
    {
        binaryParser.writeTMFrame(frame);
    }
    else
    {
        textParser.writeTMFrame(frame);
    }
}

//Record Telemetry to an output File
void C_TelemetryManager::RecordTMToFile(const QString& filename, FileFormatType format)
{
    string temp = string("Recording Telemetry to File: ") + filename.toStdString();
    debugLog->println(BCI_LOG, temp, true);

    if (format == TEXT_FILE)
    {
        outputTextFile = true;
        textParser.SetTMOutputFilename(filename);
    }
    else
    {
        outputBinaryFile = true;
        binaryParser.SetTMOutputFilename(filename);
    }
}
