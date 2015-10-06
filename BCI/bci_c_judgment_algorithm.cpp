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

#include "bci_c_telemetrymanager.h"
//Remote and Sensor Data contained within TM
void C_JudgmentAlgorithm::SetTM(TM_Frame_t *pTMFrame)
{
    C_TelemetryManager::pTMFrameMutex->acquire();
    memcpy(&mCurrentTMFrame, pTMFrame,sizeof(TM_Frame_t));
    C_TelemetryManager::pTMFrameMutex->release();
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
    ParseEEGData();
}


//Check if we are requesting an emergency stop
bool C_JudgmentAlgorithm::SafeToProceed()
{
    US_Data_t rangeData = mCurrentTMFrame.brsFrame.sensorData.rangeFinderData;
    bool safeToProceed  = true;

    //Request an Emergency Stop if We're Getting too close to an Object
    if (prevCommand == PCC_FORWARD  && rangeData.rangeFront <= EMERGENCY_STOP_DISTANCE ||
        prevCommand == PCC_BACKWARD && rangeData.rangeBack  <= EMERGENCY_STOP_DISTANCE)
    {
        emit RequestEmergencyStop();
        safeToProceed = false;
    }

    return safeToProceed;
}

//Parse the EEG Data, Update the Final Command and the Confidence Value
void C_JudgmentAlgorithm::ParseEEGData()
{

}

void C_JudgmentAlgorithm::finalizeCommand(PCC_Command_Type cmd)
{
    finalCommand      = cmd;
    commandFinalized  = true;

    emit commandReady();
}
