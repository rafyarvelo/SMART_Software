#ifndef BCI_C_BCI_PACKAGE_H
#define BCI_C_BCI_PACKAGE_H

#include "bci_c_signal_processing.h"
#include "bci_c_tm.h"
#include "bci_c_flasher_io.h"
#include "bci_c_eeg_io.h"
#include "bci_c_judgment_algorithm.h"
#include "bci_c_ja2brs.h"
#include "bci_c_ja2pcc.h"
#include "bci_c_rvs.h"
#include "bci_c_rvs2ja.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
using namespace std;

class C_BCI_Package
{
public:

    //Factory Constructor
    static C_BCI_Package* Instance();

    void startEEG();
    void sendRVS_Commands();//Call once to start sending
    void sendPCC_Commands();

private:
	 C_BCI_Package();
    ~C_BCI_Package();

    void initialize();
    bool checkFlasherComm();
    bool checkBRSHComm();
    bool checkPCCComm();
    bool checkEEGComm();

private:
    C_SignalProcessing*  pSignalProcessing;
    C_JudgmentAlgorithm* pJA;
    C_JA2BRS* 			 pJA2BRS;
	C_JA2PCC* 			 pJA2PCC;
    C_EEG_IO*            pEEG_IO;
    C_Flasher_IO*        pFlasherIO;
    C_RVS*               pRVS;
    C_TM*                pTM;

};

#endif // BCI_C_BCI_PACKAGE_H

