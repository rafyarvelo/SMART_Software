#include "bci_c_brsh_io_debug.h"

C_BRSH_IO_Debug::C_BRSH_IO_Debug()
{
}

bool C_BRSH_IO_Debug::fetchBRSFrame()
{
    currentBRSFrame = *FrameGenerator::GenerateBRSFrame();

    if (currentBRSFrame.usData.rangeToObject < EMERGENCY_STOP_DISTANCE)
    {
        emit RequestEmergencyStop();
    }
    else if (currentBRSFrame.remoteCommand != PCC_CMD_NONE)
    {
        emit remoteCommandReceived(currentBRSFrame.remoteCommand);
    }

    //Notify Listeners that a frame has arrived
    emit BRSFrameReceived(&currentBRSFrame);

    return true;//it's all good in the hood homie
}

void C_BRSH_IO_Debug::SendTMFrame(TM_Frame_t* pFrame)
{
    QString toPrint = QString("Sending BRS Frame: ") + QString::number(pFrame->timeStamp);

    debugLog->println(BRS_LOG, toPrint.toStdString());
}
