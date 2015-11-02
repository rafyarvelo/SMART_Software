#include "bci_c_judgment_algorithm.h"

C_JudgmentAlgorithm::C_JudgmentAlgorithm()
{
    debugLog = SMART_DEBUG_LOG::Instance();

    commandFinalized = false;
    prevCommand      = PCC_CMD_NONE;
    finalCommand     = PCC_CMD_NONE;
    cmdConfidence    = UNSURE;

    //Initialize Members
    memset(&mCurrentTMFrame, 0, sizeof(TM_Frame_t));
    mCurrentProcessingResult.command = PCC_CMD_NONE;
    mCurrentProcessingResult.confidence = UNSURE;
}

C_JudgmentAlgorithm::~C_JudgmentAlgorithm()
{
	
}

//Remote and Sensor Data contained within TM
void C_JudgmentAlgorithm::SetCurrentTMFrame(const TM_Frame_t& frame)
{
    mCurrentTMFrame = frame;
}

void C_JudgmentAlgorithm::SetCurrentProcessingResult(ProcessingResult_t& result)
{
    mCurrentProcessingResult = result;
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
    #ifdef ENABLE_EMERGENCY_STOP
    if (!SafeToProceed())
    {
        cmdConfidence = ABSOLUTE;
        finalizeCommand(PCC_STOP);
        return;
    }
    #endif

    //Check for Remote Command
    if (!commandFinalized && mCurrentTMFrame.brsFrame.remoteCommand != PCC_CMD_NONE)
    {
        //Update Final Command with Remote Command
        cmdConfidence = ABSOLUTE;
        finalizeCommand(mCurrentTMFrame.brsFrame.remoteCommand);
        return;
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
        debugLog->println(BCI_LOG, "Emergency Stop Requested",true);
        emit RequestEmergencyStop();
        safeToProceed = false;
    }

    return safeToProceed;
}

//Signal Processing Does the work for us, Just need to
//Update the Final Command and the Confidence Value
void C_JudgmentAlgorithm::ParseEEGData()
{
    //Update Final PCC Command From Signal Processing Result
    if (mCurrentProcessingResult.command != PCC_CMD_NONE)
    {
        cmdConfidence = mCurrentProcessingResult.confidence;
        finalizeCommand(mCurrentProcessingResult.command);
    }
}

void C_JudgmentAlgorithm::finalizeCommand(PCC_Command_Type cmd)
{
    finalCommand      = cmd;
    commandFinalized  = true;

    emit commandReady();
}
