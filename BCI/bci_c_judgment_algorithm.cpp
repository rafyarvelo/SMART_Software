#include "bci_c_judgment_algorithm.h"

C_JudgmentAlgorithm::C_JudgmentAlgorithm(C_SignalProcessing* signalProcessing)
    : mRVS_Ptr(0),
      commandSafe(false),
      prevCommand(PCC_CMD_NONE),
      finalCommand(PCC_CMD_NONE),
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
    mTMFrame = pTMFrame;
}

PCC_Command_Type C_JudgmentAlgorithm::GetFinalCommand()
{
    if (!commandSafe)
    {
        computeCommand();
    }

    prevCommand = finalCommand;
    commandSafe = false; //Make sure we recompute next time
    return finalCommand;
}

//To be effective, make sure the RVS and TM are set before calling this
Confidence_Type C_JudgmentAlgorithm::computeCommand()
{
    Confidence_Type confidence = UNSURE;


    //Let's not change the command unless we're sure
    if (confidence == UNSURE && prevCommand != PCC_CMD_NONE)
    {
        finalCommand = prevCommand;
    }

    commandSafe = true;
    return confidence;
}
