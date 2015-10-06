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

    //Start the EEG/BRS IO when the Threads are ready
    QObject::connect(pBRS_IO, SIGNAL(BRSFrameReceived(BRS_Frame_t*)),
                     this   , SLOT(onBRSFrameReceived(BRS_Frame_t*)));

    //Listen for Processed EEG Data
    QObject::connect(pSignalProcessing, SIGNAL(eegDataProcessed(C_EEG_Data&)),
                     this             , SLOT(onEEGDataProcessed(C_EEG_Data&)));

    //Listen for Emergency Stop Commands
    QObject::connect(pJA  , SIGNAL(RequestEmergencyStop()),
                     this , SLOT(onEmergencyStopRequested()));

    //Listen for the Final PCC Command
    QObject::connect(pJA  , SIGNAL(commandReady()),
                     this , SLOT(onCommandReady()));

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
    //Initialize the System
    bciState = BCI_INITIALIZATION;

    //Keep Track of the Mission Time
    stopwatch.start();

    //Make Connections to peripherals
    createConnections();

    //Configure Repetitive Visual Stimulus and send to Flasher
    pRVS->Generate();
    pJA->SetRVS(pRVS);
    pFlasherIO->SendRVS();

    //Record our TM to an output file
    #ifdef DEBUG_ONLY
    pTelemetryManager->RecordTMToFile(TM_DATA_OUTPUTFILE_BIN);
    #endif

    //Begin Thread Execution for EEG and BRS IO
    startThreads();

    debugLog->println(BCI_LOG, "Initialization Complete, Moving to STANDBY..." , true );

    //Move to Standby
    bciState = BCI_STANDBY;
    //This is all we need to do here, the Signals and Slots will
    //take care of notifying us for remote commands and Emergency Stops
    return;
}

//Go Process the Command when the EEG Data is Ready
void C_BCI_Package::onEEGDataProcessed(C_EEG_Data &eegData)
{
    processCommand();
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
    pEEG_IO->begin();
    pBRS_IO->begin();
}

void C_BCI_Package::onBRSFrameReceived(BRS_Frame_t* pFrame)
{
    processCommand();
}

//This is the entire routine of the BCI_Processing State
void C_BCI_Package::processCommand()
{
    bciState = BCI_PROCESSING;

    //Update TM and Notify the Judgement Algorithm that it's up to date
    pTelemetryManager->updateTM();
    pJA->SetTM(pTelemetryManager->GetLatestFramePtr());

    //Tell the Judgement Algorithm to Compute the Command, it will notify us when it's ready
    pJA->computeCommand();
}

void C_BCI_Package::onCommandReady()
{
    //We're Ready to send the command here
    bciState = BCI_READY;

    //Send the Command to the Power Chair Controller
    pPCC_IO->SetCommand(pJA->GetFinalCommand());
    pPCC_IO->SendCommand();

    //Move back to Standby and wait for the next command
    bciState = BCI_STANDBY;
}
