#include "bci_c_brsh_io_debug.h"

C_BRSH_IO_Debug::C_BRSH_IO_Debug()
{
}

bool C_BRSH_IO_Debug::fetchBRSFrame()
{
    //Generate a Random Frame
    pLatestBRSFrame = FrameGenerator::GenerateBRSFrame(pLatestBRSFrame, FrameGenerator::GeneratePCC_Command());

    //Lock the BRS Frame From Concurrent Access
    pBRSFrameMutex->acquire(BRS_FRAME_MUTEX);

    //Notify Listeners that a frame has arrived
    emit BRSFrameReceived(pLatestBRSFrame);

    //Unlock the BRS Frame
    pBRSFrameMutex->release(BRS_FRAME_MUTEX);

    return true;//it's all good in the hood homie
}

void C_BRSH_IO_Debug::SendTMFrame(TM_Frame_t* pFrame)
{
    SMART_DEBUG_LOG* debugLog = SMART_DEBUG_LOG::Instance();

    QString toPrint = QString("Sending BRS Frame: ") + QString::number(pFrame->timeStamp);
    debugLog->println(BRS_LOG, toPrint.toStdString());
}
