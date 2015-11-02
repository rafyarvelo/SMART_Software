#ifndef BCI_C_JUDGMENT_ALGORITHM_H
#define BCI_C_JUDGMENT_ALGORITHM_H

#include "../CCS_PROJECTS/PCC/power_chair_command_constants.h"
#include "../smart_debug_log.h"
#include "../smart_config.h"
#include "bci_c_signal_processing.h"
#include "bci_c_telemetrymanager.h"
#include "bci_c_rvs.h"

//Forward Declaration of Telemetry Manager Class
class C_TelemetryManager;

class C_JudgmentAlgorithm : public QObject, public C_Singleton<C_JudgmentAlgorithm>
{
    Q_OBJECT
public:
	//The Signal Processing class with deliver the processed data to the JA
     C_JudgmentAlgorithm();
	~C_JudgmentAlgorithm();

    PCC_Command_Type GetFinalCommand();

    //To be effective, make sure the RVS and TM are set before calling this
    void computeCommand();

    //Check for an Emergency Stop
    bool SafeToProceed();

    //Give the Telemetry Manager Private Access to our Variables
    friend class C_TelemetryManager;

public slots:
    void SetCurrentTMFrame(const TM_Frame_t& frame);
    void SetCurrentProcessingResult(ProcessingResult_t& result);

signals:
    void RequestEmergencyStop();
    void commandReady();

private:
    void ParseEEGData();
    void finalizeCommand(PCC_Command_Type cmd);

private:
    SMART_DEBUG_LOG* debugLog;

    //We Maintain our own local copies of the TM
    //Frame and the Signal Processing Result so they can be updated independently
    TM_Frame_t          mCurrentTMFrame;
    ProcessingResult_t  mCurrentProcessingResult;

    PCC_Command_Type    finalCommand;
    PCC_Command_Type    prevCommand;
    Confidence_Type     cmdConfidence;

    bool commandFinalized; //Just in case user forgets to call computeCommand()
};

#endif // BCI_C_JUDGMENT_ALGORITHM_H

