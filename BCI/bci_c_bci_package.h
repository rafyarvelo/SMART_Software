#ifndef BCI_C_BCI_PACKAGE_H
#define BCI_C_BCI_PACKAGE_H

#include <QTime>
#include "bci_c_signal_processing.h"
#include "bci_c_tm.h"
#include "bci_c_flasher_io.h"
#include "bci_c_eeg_io.h"
#include "bci_c_eeg_io_debug.h"
#include "bci_c_eeg_io_emotiv.h"
#include "bci_c_eeg_io_nautilus.h"
#include "bci_c_judgment_algorithm.h"
#include "bci_c_brsh_io.h"
#include "bci_c_pcc_io.h"
#include "bci_c_rvs.h"

#define COMMAND_TIMEOUT 5000 //ms
#define MAX_MISSES      5    //Max Time outs before quitting

typedef enum BCIState
{
    BCI_OFF=0,
    /*
     * 1) Remain in this state until Run() is called
     * 2) When Run() is called, Move to BCI_INITIALIZATION
     */
    BCI_INITIALIZATION,
    /*
     * 1) Create Instances of RVS, JA2BRS, BRS2JA, JA2PCC, and EEG IO
     * 2) Connect to Flasher, EEG, BRSH, and PCC
     * 3) Generate RVS Frequencies
     * 4) Send RVS to Flasher
     * 5) Send TM Packet to BRSH
     * 6) Move to BCI_STANDBY
     */
    BCI_STANDBY,
    /*
     * 1) Wait for EEG Data or Remote Commands
     * 2) Upon Receipt of EEG Data or Remote Commands, move to BCI_PROCESSING
     */
    BCI_PROCESSING,
    /*
     * 1) Process the data
     * 2) Generate PCC Command
     * 3) Move to BCI_READY
     */
    BCI_READY
    /*
     * 1) Send the Command
     * 2) Revert to BCI_STANDBY
     */
}BCIState;

class C_BCI_Package
{
public:

    //Factory Constructor
    static C_BCI_Package* Instance();

    //Infinite Loop when called
    void Run();

private:
	 C_BCI_Package();
    ~C_BCI_Package();
    void initialize();
    bool checkConnections();
    void startEEG();
    C_EEG_IO* createEEG_IO(eegTypeEnum type=DEFAULT_EEG_TYPE);//factory EEG Construction

private slots:
    void onEEGDataReady(EEG_Data* pData);

private:
	SMART_DEBUG_LOG*     debugLog;
    C_SignalProcessing*  pSignalProcessing;
    C_JudgmentAlgorithm* pJA;
    C_PCC_IO* 			 pPCC_IO;
    C_BRSH_IO*           pBRS_IO;
    C_EEG_IO*            pEEG_IO;
    C_Flasher_IO*        pFlasherIO;
    C_RVS*               pRVS;

    //Connection Status
    ConnectionStatusType eegConnectionStatus;
    ConnectionStatusType flasherConnectionStatus;
    ConnectionStatusType brshConnectionStatus;
    ConnectionStatusType pccConnectionStatus;

    //Used to control event loop
    BCIState bciState;

    //If our comm is slow, lets reuse the last command up till a pre-defined miss_count
    QTime        stopwatch;
    unsigned int missCount;

    EEG_Data* eegDataBuffer;
    C_TM*     tmDataBuffer;
};

#endif // BCI_C_BCI_PACKAGE_H

