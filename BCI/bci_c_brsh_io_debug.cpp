#include "bci_c_brsh_io_debug.h"

C_BRSH_IO_Debug::C_BRSH_IO_Debug()
{
}

bool C_BRSH_IO_Debug::fetchBRSFrame()
{
    //Generate a Random Frame
    FrameGenerator::GenerateBRSFrame(FrameGenerator::GeneratePCC_Command(), &mCurrentBRSFrame);

    //Put the frame in our buffer
    brsFrameBuffer.Put(mCurrentBRSFrame);

    //Notify Listeners that a frame has arrived
    emit BRSFrameReceived(&brsFrameBuffer);

    return true;//it's all good in the hood homie
}

void C_BRSH_IO_Debug::SendTMFrame(tmFrameBufferType* pBuffer)
{
    SMART_DEBUG_LOG* debugLog = SMART_DEBUG_LOG::Instance();
    TM_Frame_t frame = pBuffer->Get();

    QString toPrint = QString("Sending TM Frame: ") + QString::number(frame.timeStamp);
    debugLog->println(BRS_LOG, toPrint.toStdString());
}
