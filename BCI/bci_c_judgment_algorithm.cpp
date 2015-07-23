#include "bci_c_judgment_algorithm.h"

C_JudgmentAlgorithm::C_JudgmentAlgorithm(C_SignalProcessing* signalProcessing)
{
	mSignalProcessingPtr = signalProcessing;
	
	pJA2BRS = C_JA2BRS::Instance();
	pJA2PCC = C_JA2PCC::Instance();
}

C_JudgmentAlgorithm::~C_JudgmentAlgorithm()
{
	
}
