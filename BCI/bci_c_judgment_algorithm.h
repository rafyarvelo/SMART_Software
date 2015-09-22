#ifndef BCI_C_JUDGMENT_ALGORITHM_H
#define BCI_C_JUDGMENT_ALGORITHM_H

#include "../PCC/power_chair_command_constants.h"
#include "../smart_debug_log.h"
#include "../smart_config.h"
#include "bci_c_signal_processing.h"
#include "bci_c_rvs.h"
#include "bci_c_tm.h"

//Use to see how sure we are that the command is correct
typedef enum Confidence_Type
{
    UNSURE=0,
    MODERATE,
    LIKELY,
    ABSOLUTE
}Confidence_Type;

class C_JudgmentAlgorithm : public QObject
{
    Q_OBJECT
public:
	//The Signal Processing class with deliver the processed data to the JA
	 C_JudgmentAlgorithm(C_SignalProcessing* signalProcessing);
	~C_JudgmentAlgorithm();
	
	static C_JudgmentAlgorithm* Instance(C_SignalProcessing* signalProcessing)
	{
		return new C_JudgmentAlgorithm(signalProcessing);
	}


    PCC_Command_Type GetFinalCommand();

    //To be effective, make sure the RVS and TM are set before calling this
    void computeCommand();

    //Check for an Emergency Stop
    bool SafeToProceed();

public slots:
    void SetRVS(C_RVS* pRVS);
    void SetTM (TM_Frame_t* pTMFrame);

signals:
    void commandReady(PCC_Command_Type& cmd);

private:
    PCC_Command_Type ParseEEGData();
    void finalizeCommand(PCC_Command_Type cmd);

	C_SignalProcessing* mSignalProcessingPtr;
    C_RVS*              mRVS_Ptr;
    TM_Frame_t          mCurrentTMFrame;


    PCC_Command_Type    finalCommand;
    PCC_Command_Type    prevCommand;
    Confidence_Type     cmdConfidence;

    bool commandFinalized; //Just in case user forgets to call computeCommand()
};

#endif // BCI_C_JUDGMENT_ALGORITHM_H

