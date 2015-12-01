#include "bci_c_judgment_algorithm.h"

C_JudgmentAlgorithm::C_JudgmentAlgorithm()
{
    debugLog = SMART_DEBUG_LOG::Instance();

    commandFinalized = false;
    prevCommand      = PCC_CMD_NONE;
    finalCommand     = PCC_CMD_NONE;
    cmdConfidence    = UNSURE;
    prevCmdSource    = CMD_SRC_NONE;

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
    std::cout << "=================\n\n\n\n LAST CMD SOURCE = " << (unsigned int) prevCmdSource << "\n\n\n\n\n\n================= "<< std::endl;

    //Check for Emergency Stop
    #ifdef ENABLE_EMERGENCY_STOP
    if (!SafeToProceed())
    {
        cmdConfidence = ABSOLUTE;
        prevCmdSource = CMD_SRC_SENSOR;
        finalizeCommand(PCC_STOP);
        return;
    }
    #endif

    //Check for Remote Command
    if (!commandFinalized && mCurrentTMFrame.brsFrame.remoteCommand != PCC_CMD_NONE)
    {
        //Update Final Command with Remote Command
        cmdConfidence = ABSOLUTE;
        prevCmdSource = CMD_SRC_REMOTE;
        finalizeCommand(mCurrentTMFrame.brsFrame.remoteCommand);
        return;
    }
    
    //Process EEG Data
    switch (prevCmdSource)
    {
        case CMD_SRC_NONE:
        case CMD_SRC_SENSOR:
        case CMD_SRC_SP:
            ParseEEGData();
            break;
        case CMD_SRC_REMOTE:
            if (prevCommand == PCC_STOP)
            {
                ParseEEGData();
            }
            else
            {
                //Ignore EEG Command, Remote Override is still running
            }
            break;
    }
}


//Check if we are requesting an emergency stop
bool C_JudgmentAlgorithm::SafeToProceed()
{
    US_Data_t rangeData = mCurrentTMFrame.brsFrame.sensorData.rangeFinderData;
    bool safeToProceed  = true;

    //Request an Emergency Stop if We're Getting too close to an Object
    if (rangeData.rangeFront <= EMERGENCY_STOP_DISTANCE)
    {
        switch (prevCommand)
        {
            case PCC_RIGHT:
            case PCC_LEFT:
            case PCC_BACKWARD:
            case PCC_STOP:
                safeToProceed  = true;
                break;
            case PCC_FORWARD:
                safeToProceed = false;
                prevCmdSource = CMD_SRC_SENSOR;
                return safeToProceed;
        }
    }
    
    if (rangeData.rangeBack <= EMERGENCY_STOP_DISTANCE)
    {
        switch (prevCommand)
        {
            case PCC_RIGHT:
            case PCC_LEFT:
            case PCC_FORWARD:
            case PCC_STOP:
                safeToProceed  = true;
                break;
            case PCC_BACKWARD:
                safeToProceed = false;
                prevCmdSource = CMD_SRC_SENSOR;
                return safeToProceed;
        }
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
        prevCmdSource = CMD_SRC_SP;
        finalizeCommand(mCurrentProcessingResult.command);
    }
}

void C_JudgmentAlgorithm::finalizeCommand(PCC_Command_Type cmd)
{
    finalCommand      = cmd;
    commandFinalized  = true;

    emit commandReady();
}
