#ifndef BCI_C_BCI_PACKAGE_H
#define BCI_C_BCI_PACKAGE_H

#include "bci_c_signal_processing.h"
#include "bci_c_tm.h"
#include "../io/bci_c_flasher_io.h"
#include "../io/bci_c_eeg_io.h"
#include "../JA/bci_c_judgment_algorithm.h"
#include "../JA/bci_c_ja2brs.h"
#include "../JA/bci_c_ja2pcc.h"
#include "../ssvep/bci_c_rvs.h"
#include "../ssvep/bci_c_rvs2ja.h"

class C_BCI_Package
{
public:
     C_BCI_Package();
    ~C_BCI_Package();

    void startEEG();
    void sendRVS_Commands();
    void sendPCC_Commands();

private:
    C_SignalProcessing  signalProcessing;
    C_JudgmentAlgorithm ja;
    C_EEG_IO            eegIO;
    C_Flasher_IO        flasherIO;
    C_RVS               rvs;
};

#endif // BCI_C_BCI_PACKAGE_H

