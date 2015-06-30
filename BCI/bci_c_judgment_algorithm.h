#ifndef BCI_C_JUDGMENT_ALGORITHM_H
#define BCI_C_JUDGMENT_ALGORITHM_H

#include "bci_c_ja2brs.h"
#include "bci_c_ja2pcc.h"
#include "bci_c_signal_processing.h"
#include "../PCC/power_chair_command_constants.h"

class C_JudgmentAlgorithm
{
public:
	//The Signal Processing class with deliver the processed data to the JA
	 C_JudgmentAlgorithm(C_SignalProcessing* signalProcessing);
	~C_JudgmentAlgorithm();

private:
	C_SignalProcessing* mSignalProcessingPtr;
};

#endif // BCI_C_JUDGMENT_ALGORITHM_H

