/*
 * bci_c_bci_package.cpp
 *
 *  Created on: Jul 15, 2015
 *      Author: Rafy
 */

#include "bci_c_bci_package.h"

C_BCI_Package::C_BCI_Package()
    : bciState(BCI_OFF)
{
    debugLog = SMART_DEBUG_LOG::Instance();
    debugLog->BCI_Log() << "Instantiating BCI Package..." << endl;

    //Repetitive Visual Stimulus API
    pRVS              = C_RVS::Instance();
    pFlasherIO        = C_Flasher_IO::Instance(pRVS);

    //Device Input/Output
    pEEG_IO           = createEEG_IO(DEFAULT_EEG_TYPE);
    pBRS_IO           = createBRS_IO(DEFAULT_BRS_TYPE);
    pPCC_IO           = createPCC_IO(DEFAULT_PCC_TYPE);

    //Other Classes with dependencies
    pSignalProcessing = C_SignalProcessing ::Instance(pEEG_IO);
    pJA               = C_JudgmentAlgorithm::Instance(pSignalProcessing);
    pTelemetryManager = C_TelemetryManager ::Instance(this, pEEG_IO,pBRS_IO, pRVS);

    //Connection Status of Peripherals
    eegConnectionStatus      = NOT_CONNECTED;
    flasherConnectionStatus  = NOT_CONNECTED;
    pccConnectionStatus      = NOT_CONNECTED;
    brshConnectionStatus     = NOT_CONNECTED;

    //Listen for Processed EEG Data
    QObject::connect(pSignalProcessing, SIGNAL(eegDataProcessed(C_EEG_Data&)),
                     this             , SLOT(onEEGDataProcessed(C_EEG_Data&)));

    //Listen for Emergency Stop Commands
    QObject::connect(pBRS_IO , SIGNAL(RequestEmergencyStop()),
                     this    , SLOT(onEmergencyStopRequested()));

    //Listen for Remote Commands
    QObject::connect(pBRS_IO , SIGNAL(remoteCommandReceived(PCC_Command_Type&)),
                     this    , SLOT(onRemoteCmdReceived(PCC_Command_Type&)));

    debugLog->println(BCI_LOG, "BCI Package Instantiated Successfully", true);
}

C_BCI_Package::~C_BCI_Package()
{
    if (pEEG_IO)
    {
        delete pEEG_IO;
    }
    if (pSignalProcessing)
    {
        delete pSignalProcessing;
    }
    if (pJA)
    {
        delete pJA;
    }
    if (pFlasherIO)
    {
        delete pFlasherIO;
    }
    if (pRVS)
    {
        delete pRVS;
    }
}

void C_BCI_Package::createConnections()
{
    debugLog->println(BCI_LOG, "Connecting to Flasher...", true);
    flasherConnectionStatus = pFlasherIO->connect();

    debugLog->println(BCI_LOG, "Connecting to EEG..." , true );
    eegConnectionStatus = pEEG_IO->connect();

    debugLog->println(BCI_LOG, "Connecting to BRSH..." , true );
    brshConnectionStatus = pBRS_IO->connect();

    debugLog->println(BCI_LOG, "Connecting to PCC..." , true );
    pccConnectionStatus = pPCC_IO->connect();
}

C_EEG_IO* C_BCI_Package::createEEG_IO(eegTypeEnum type)
{
	C_EEG_IO* ptr = 0;
	
	switch (type)
	{
        case EEG_TYPE_NAUTILUS:
            ptr = C_EEG_IO_NAUTILUS::Instance();
        break;

        case EEG_TYPE_DEBUG:
            ptr = C_EEG_IO_DEBUG::Instance();
        break;

        //Emotiv is the default
        default:
            ptr = C_EEG_IO_EMOTIV::Instance();
        break;
	}
	
	return ptr;
}

C_BRSH_IO* C_BCI_Package::createBRS_IO(brsTypeEnum type)
{
    C_BRSH_IO* ptr = 0;

    switch (type)
    {
        case BRS_TYPE_TIVA:
            ptr = C_BRSH_IO_Serial::Instance();
        break;
        default:
            ptr = C_BRSH_IO_Debug::Instance();
        break;
    }

    return ptr;
}

C_PCC_IO* C_BCI_Package::createPCC_IO(pccTypeEnum type)
{
    C_PCC_IO* ptr = 0;

    switch (type)
    {
        case PCC_TYPE_MSP:
            ptr = C_PCC_IO_Serial::Instance();
        break;
        default:
            ptr = C_PCC_IO_Debug::Instance();
        break;
    }

    return ptr;
}

bool C_BCI_Package::checkConnections()
{
    ConnectionStatusType status = CONNECTED;

    if (bciState <= BCI_INITIALIZATION)
    {
        return CONNECTED;//We haven't initialized yet, so just return true.
    }

    //Check EEG Connection Status...
    if (!eegConnectionStatus)
    {
        debugLog->println(BCI_LOG, "EEG Connection not created! Stopping BCI..." , false, true );
        status = NOT_CONNECTED;
    }

    //Check PCC Connection Status...
    if (!pccConnectionStatus)
    {
    debugLog->println(BCI_LOG, "PCC Disconnected! Stopping BCI..." , false, true );
        status = NOT_CONNECTED;
    }


    //Check BRSH Connection Status...
    if (!brshConnectionStatus)
    {
        debugLog->println(BCI_LOG, "BRSH Disconnected! Stopping BCI..." , false, true );
        status = NOT_CONNECTED;
    }

    //Check EEG Connection Status...
    if (!flasherConnectionStatus)
    {
        debugLog->println(BCI_LOG, "Flasher Disconnected! Stopping BCI..." , false, true );
        status = NOT_CONNECTED;
    }

    return status;
}


void C_BCI_Package::Run()
{
    switch (bciState)
    {
    case BCI_OFF:

        //Start initializing!
        goToState(BCI_INITIALIZATION);
        break;

    case BCI_INITIALIZATION:

        //Keep Track of the Mission Time
        stopwatch.start();

        //Make Connections to peripherals
        createConnections();

        //Configure Repetitive Visual Stimulus and send to Flasher
        pRVS->Generate();
        pJA->SetRVS(pRVS);
        pFlasherIO->SendRVS();

        //Record our TM to an output file
        pTelemetryManager->RecordTMToFile(TM_DATA_OUTPUTFILE_BIN);

        //Begin Thread Execution for EEG and BRS IO
        startThreads();

        debugLog->println(BCI_LOG, "Initialization Complete, Moving to STANDBY..." , true );

        //Wait for Data in Standby
        goToState(BCI_STANDBY);

        break;

    case BCI_STANDBY:

        //This is all we need to do here, the Signals and Slots will
        //take care of notifying us for remote commands and Emergency Stops
        break;

    case BCI_PROCESSING:

        //Update the Judgment Algorithm with the current data
        pJA->SetTM(pTelemetryManager->GetLatestFramePtr());

        //Decide Final Power Chair Command
        pJA->computeCommand();

        //Move to Ready to send the command
        goToState(BCI_READY);

        break;

    case BCI_READY:

        //Send the PCC Command then Revert to STANDBY
        pPCC_IO->SetCommand(pJA->GetFinalCommand());
        pPCC_IO->SendCommand();

        goToState(BCI_STANDBY);
        break;
    }
}

//Slots
void C_BCI_Package::onEEGDataProcessed(C_EEG_Data &eegData)
{
    //Go Process the Command
    goToState(BCI_PROCESSING);
}

void C_BCI_Package::onRemoteCmdReceived(PCC_Command_Type& cmd)
{
    //Update TM with the command, then let the
    //Judgment Algorithm take care of the rest
    pTelemetryManager->updateTM();
    goToState(BCI_PROCESSING);
}

void C_BCI_Package::onEmergencyStopRequested()
{
    //No messing around here, just send the command
    pPCC_IO->SetCommand(PCC_STOP);
    pPCC_IO->SendCommand();
}

//Begin EEG and BRS IO Tasks
void C_BCI_Package::startThreads()
{
    pEEG_IO->start();
    pBRS_IO->start();
}

void C_BCI_Package::goToState(BCIState state)
{
    bciState = state;
    Run();
}

////Relax in BCI_Package Standby
//void C_BCI_Package::standby( int millisecondsToWait )
//{
//    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
//    while( QTime::currentTime() < dieTime )
//    {
//        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
//    }
//}
