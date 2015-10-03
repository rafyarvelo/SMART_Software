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
    void startThreads();
    void goToState(BCIState state);

    //Factory Constructors for our IO Classes
    C_EEG_IO*  createEEG_IO(eegTypeEnum type=DEFAULT_EEG_TYPE);
    C_BRSH_IO* createBRS_IO(brsTypeEnum type=DEFAULT_BRS_TYPE);
    C_PCC_IO*  createPCC_IO(pccTypeEnum type=DEFAULT_PCC_TYPE);

private slots:
    void onEEGDataProcessed(C_EEG_Data& data);
    void onCommandReady();
    void onEmergencyStopRequested();

private:
    void processCommand();

	SMART_DEBUG_LOG*     debugLog;
    C_SignalProcessing*  pSignalProcessing;
    C_JudgmentAlgorithm* pJA;
    C_PCC_IO* 			 pPCC_IO;
    C_BRSH_IO*           pBRS_IO;
    C_EEG_IO*            pEEG_IO;
    C_Flasher_IO*        pFlasherIO;
    C_RVS*               pRVS;
    C_TelemetryManager*  pTelemetryManager;

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

