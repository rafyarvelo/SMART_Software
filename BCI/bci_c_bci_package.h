#ifndef BCI_C_BCI_PACKAGE_H
#define BCI_C_BCI_PACKAGE_H

//Forward Declaration
class C_TelemetryManager;

#include <QTime>
#include <QtCore>
#include "bci_c_rvs.h"
#include "bci_c_flasher_io.h"
#include "bci_c_signal_processing.h"
#include "bci_c_eeg_io_debug.h"
#include "bci_c_eeg_io_emotiv.h"
#include "bci_c_eeg_io_nautilus.h"
#include "bci_c_brsh_io_serial.h"
#include "bci_c_brsh_io_debug.h"
#include "bci_c_judgment_algorithm.h"
#include "bci_c_telemetrymanager.h"
#include "bci_c_pcc_io_serial.h"
#include "bci_c_pcc_io_debug.h"

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

class C_BCI_Package : public QObject , public C_Singleton<C_BCI_Package>
{
    Q_OBJECT
public:
    C_BCI_Package();
   ~C_BCI_Package();

    //Main Program Execution here, Begins an infinite state machine
    void Run();

private:
    void createConnections();
    bool checkConnections();
    void standby( int millisecondsToWait );

    //Factory Constructors for our IO Classes
    C_EEG_IO*  createEEG_IO(eegTypeEnum type=DEFAULT_EEG_TYPE);
    C_BRSH_IO* createBRS_IO(brsTypeEnum type=DEFAULT_BRS_TYPE);
    C_PCC_IO*  createPCC_IO(pccTypeEnum type=DEFAULT_PCC_TYPE);

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
    C_TelemetryManager*  pTelemetryManager;

    //Thread Execution
    C_EEG_IO_Task*       pEEG_IO_Task; //50 Hz
    C_BRSH_IO_Task*      pBRS_IO_Task; //10 Hz

    //Used to control event loop
    BCIState bciState;

    //Mission Time
    QTime stopwatch;

    //Connection Status
    ConnectionStatusType eegConnectionStatus;
    ConnectionStatusType flasherConnectionStatus;
    ConnectionStatusType brshConnectionStatus;
    ConnectionStatusType pccConnectionStatus;

    //buffer to hold some debug prints
    QString consoleOutput;

    //Allow Telemetry Manager to access private variables
    friend class C_TelemetryManager;
};

#endif // BCI_C_BCI_PACKAGE_H

