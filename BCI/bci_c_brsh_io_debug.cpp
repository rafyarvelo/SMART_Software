#include "bci_c_brsh_io_debug.h"

C_BRSH_IO_Debug::C_BRSH_IO_Debug()
{
}

bool C_BRSH_IO_Debug::fetchBRSFrame()
{
    BRS_Frame_t* frame = 0;

    //Generate a Random Frame
    frame = FrameGenerator::GenerateBRSFrame(FrameGenerator::GeneratePCC_Command());

    //Notify Listeners that a frame has arrived
    emit BRSFrameReceived(frame);

    return true;//it's all good in the hood homie
}

void C_BRSH_IO_Debug::SendTMFrame(TM_Frame_t* pFrame)
{
    SMART_DEBUG_LOG* debugLog = SMART_DEBUG_LOG::Instance();

    QString toPrint = QString("Sending TM Frame: ") + QString::number(pFrame->timeStamp);
    debugLog->println(BRS_LOG, toPrint.toStdString());
}
