#include "bci_c_judgment_algorithm.h"

C_JudgmentAlgorithm::C_JudgmentAlgorithm(C_SignalProcessing* signalProcessing)
    : mRVS_Ptr(0),
      commandFinalized(false),
      prevCommand(PCC_CMD_NONE),
      finalCommand(PCC_CMD_NONE),
      cmdConfidence(UNSURE),
      mSignalProcessingPtr(signalProcessing)
{

}

C_JudgmentAlgorithm::~C_JudgmentAlgorithm()
{
	
}

void C_JudgmentAlgorithm::SetRVS(C_RVS* pRVS)
{
    mRVS_Ptr = pRVS;
}

//Remote and Sensor Data contained within TM
void C_JudgmentAlgorithm::SetTM(TM_Frame_t *pTMFrame)
{
    memcpy(&mCurrentTMFrame, pTMFrame,sizeof(TM_Frame_t));
}

PCC_Command_Type C_JudgmentAlgorithm::GetFinalCommand()
{
    if (!commandFinalized)
    {
        computeCommand();
    }

    prevCommand      = finalCommand;
    commandFinalized = false; //Make sure we recompute next time
    return finalCommand;
}

//To be effective, make sure the RVS and TM are set before calling this
void C_JudgmentAlgorithm::computeCommand()
{
    //Check for Emergency Stop
    if (!SafeToProceed())
    {
        cmdConfidence = ABSOLUTE;
        finalizeCommand(PCC_STOP);
    }

    //Check for Remote Command
    else if (!commandFinalized && mCurrentTMFrame.brsFrame.remoteCommand != PCC_CMD_NONE)
    {
        //Update Final Command with Remote Command
        cmdConfidence = ABSOLUTE;
        finalizeCommand(mCurrentTMFrame.brsFrame.remoteCommand);
    }

    //Process EEG Data
    else
    {
        finalizeCommand(ParseEEGData());
    }
}


//Check if we are requesting an emergency stop
bool C_JudgmentAlgorithm::SafeToProceed()
{
    return (mCurrentTMFrame.brsFrame.usData.rangeToObject > EMERGENCY_STOP_DISTANCE);
}

//Parse the EEG Data, Update the Final Command and the Confidence Value
PCC_Command_Type C_JudgmentAlgorithm::ParseEEGData()
{
    //Let's not change the command unless we're sure
    if (cmdConfidence > UNSURE && prevCommand != PCC_CMD_NONE)
    {
        return prevCommand;
    }

    else
    {
        //LOL
        if (mSignalProcessingPtr->GetProcessedData().GetFramePtr()->counter % 7 == 0)
        {
            return PCC_FORWARD;
        }
        else
        {
            return PCC_RIGHT;
        }
    }
}

void C_JudgmentAlgorithm::finalizeCommand(PCC_Command_Type cmd)
{
    finalCommand      = cmd;
    commandFinalized  = true;
    emit commandReady(finalCommand);
}
