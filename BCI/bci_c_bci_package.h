#ifndef BCI_C_BCI_PACKAGE_H
#define BCI_C_BCI_PACKAGE_H

//Forward Declaration
class C_TelemetryManager;

#include <QTime>
#include "bci_c_signal_processing.h"
#include "bci_c_tm.h"
#include "bci_c_flasher_io.h"
#include "bci_c_eeg_io.h"
#include "bci_c_eeg_io_debug.h"
#include "bci_c_eeg_io_emotiv.h"
#include "bci_c_eeg_io_nautilus.h"
#include "bci_c_judgment_algorithm.h"
#include "bci_c_brsh_io_serial.h"
#include "bci_c_brsh_io_debug.h"
#include "bci_c_telemetrymanager.h"
#include "bci_c_pcc_io.h"
#include "bci_c_rvs.h"

#define COMMAND_TIMEOUT 20000 //Wait 20 seconds before killing the System

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

class C_BCI_Package : public QObject
{
    Q_OBJECT
public:

    //Factory Constructor
    static C_BCI_Package* Instance();

    //Infinite Loop when called
    void Run();

    QTime& currentTime() { return stopwatch; }

    //Connection Status
    ConnectionStatusType eegConnectionStatus;
    ConnectionStatusType flasherConnectionStatus;
    ConnectionStatusType brshConnectionStatus;
    ConnectionStatusType pccConnectionStatus;
private:
	 C_BCI_Package();
    ~C_BCI_Package();
    void initialize();
    void updateTM();
    bool checkConnections();
    void startEEG();
    C_EEG_IO*  createEEG_IO(eegTypeEnum type=DEFAULT_EEG_TYPE);//factory EEG Construction
    C_BRSH_IO* createBRS_IO(brsTypeEnum type=DEFAULT_BRS_TYPE);//factory BRS Construction

private slots:
    void onEEGDataProcessed(C_EEG_Data& data);
    void onRemoteCmdReceived(PCC_Command_Type& cmd);
    void onEmergencyStopRequested();

private:
	SMART_DEBUG_LOG*     debugLog;
    C_SignalProcessing*  pSignalProcessing;
    C_JudgmentAlgorithm* pJA;
    C_PCC_IO* 			 pPCC_IO;
    C_BRSH_IO*           pBRS_IO;
    C_EEG_IO*            pEEG_IO;
    C_Flasher_IO*        pFlasherIO;
    C_RVS*               pRVS;

    //Used to control event loop
    BCIState bciState;

    //Mission Time
    QTime stopwatch;

    //Manage the Telemetry Stream
    C_TelemetryManager* pTelemetryManager;
    TM_Frame_t*         pLatestTM_Frame;
};

#endif // BCI_C_BCI_PACKAGE_H

