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
    //Setup Debug Logging
    createDebugDirectory();
    debugLog = SMART_DEBUG_LOG::Instance();
    debugLog->BCI_Log() << "Instantiating BCI Package..." << endl;

    //Repetitive Visual Stimulus API
    pRVS = C_RVS::Instance();

    //Device Input/Output
    pEEG_IO           = createEEG_IO(DEFAULT_EEG_TYPE);
    pBRS_IO           = createBRS_IO(DEFAULT_BRS_TYPE);
    pPCC_IO           = createPCC_IO(DEFAULT_PCC_TYPE);
    pFlasherIO        = createFlasher_IO(DEFAULT_FLASHER_TYPE);

    //Signal Processing, Judgement Algorithm, and Telemetry Management
    pSignalProcessing = C_SignalProcessing ::Instance();
    pJA               = C_JudgmentAlgorithm::Instance();
    pTelemetryManager = C_TelemetryManager ::Instance(this, pEEG_IO, pBRS_IO, pRVS, pPCC_IO, pJA);

    //Connection Status of Peripherals
    eegConnectionStatus      = NOT_CONNECTED;
    flasherConnectionStatus  = NOT_CONNECTED;
    pccConnectionStatus      = NOT_CONNECTED;
    brshConnectionStatus     = NOT_CONNECTED;

    //Process Commands when BRS Frames are Received
    QObject::connect(pBRS_IO, SIGNAL(BRSFrameReceived(brsFrameBufferType*)),
                     this   , SLOT(onBRSFrameReceived(brsFrameBufferType*)));

    //Process EEG Frames as they become available
    QObject::connect(pEEG_IO, SIGNAL(EEGFrameReceived(eegFrameBufferType*)),
                     this   , SLOT(onEEGFrameReceived(eegFrameBufferType*)));

    //Listen for Processed EEG Data
    QObject::connect(pSignalProcessing, SIGNAL(eegDataProcessed(resultsBufferType*)),
                     this             , SLOT(onEEGDataProcessed(resultsBufferType*)));

    //Listen for Emergency Stop Commands, and act on them immediately
    QObject::connect(pJA  , SIGNAL(RequestEmergencyStop()),
                     this , SLOT(onEmergencyStopRequested()), Qt::DirectConnection);

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

        #ifdef EMOTIV
        case EEG_TYPE_EMOTIV:
            ptr = C_EEG_IO_EMOTIV::Instance();
        break;
        #endif

        default:
            ptr = C_EEG_IO_DEBUG::Instance();
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

C_Flasher_IO* C_BCI_Package::createFlasher_IO(flasherTypeEnum type)
{
    C_Flasher_IO* ptr = 0;

    switch (type)
    {
        case FLASHER_TYPE_ATMEL:
            ptr = C_Flasher_IO_GPIO::Instance();
        break;
        default:
            ptr = C_Flasher_IO_Debug::Instance();
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
    pFlasherIO->SendRVS();

    //Record our TM to an output file
    pTelemetryManager->RecordTMToFile(TM_DATA_OUTPUTFILE_BIN);
    pEEG_IO->RecordTMToFile(EEG_DATA_OUTPUTFILE_BIN);

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
void C_BCI_Package::onEEGDataProcessed(resultsBufferType* pResults)
{
    bool status = false;
    ProcessingResult_t result;

    if (pResults)
    {
        //Get Any Available results from the buffer
        result = pResults->Get(&status);

        if (status)
        {
            //Update the Judgement Algorithm with the latest processing Result and
            //Process the command
            pJA->SetCurrentProcessingResult(result);
            processCommand();
        }
    }
}

//Get a Frame from the EEG IO Buffer When Signaled
void C_BCI_Package::onEEGFrameReceived(eegFrameBufferType* pBuffer)
{
    bool status = false;
    EEG_Frame_t frame;

    if (pBuffer)
    {
        //Try to Get a Frame from the Buffer
        frame = pBuffer->Get(&status);

        if (status)
        {
            //Tell Signal Processing to Go Process the Frame
            pSignalProcessing->processFrame(frame);
        }
    }
}

//Get a Frame from the BRS IO Buffer When Signaled
void C_BCI_Package::onBRSFrameReceived(brsFrameBufferType* pBuffer)
{
    bool status = false;
    BRS_Frame_t frame;

    if (pBuffer)
    {
        //Try to Get a Frame from the Buffer
        frame = pBuffer->Get(&status);

        if (status)
        {
            //Update TM and Notify the Judgement Algorithm that it's up to date
            pTelemetryManager->updateTM(frame);
            processCommand();
        }
    }
}

void C_BCI_Package::onEmergencyStopRequested()
{
    //No messing around here, just send the command
    pPCC_IO->EmergencyStop();
}

//Begin EEG and BRS IO Tasks
void C_BCI_Package::startThreads()
{
    #ifdef USE_EEG_DATA
    pEEG_IO->begin();
    #endif

    pBRS_IO->begin();
    pFlasherIO->begin();        
}

//This is the entire routine of the BCI_Processing State
void C_BCI_Package::processCommand()
{
    bciState = BCI_PROCESSING;

    pJA->SetCurrentTMFrame(pTelemetryManager->GetLatestFrame());

    //Tell the Judgement Algorithm to Compute the Command, it will notify us when it's ready
    pJA->computeCommand();
}

void C_BCI_Package::onCommandReady()
{
    //We're Ready to send the command here
    bciState = BCI_READY;

    //Send the Command to the Power Chair Controller
    pPCC_IO->SendCommand(pJA->GetFinalCommand());

    //Move back to Standby and wait for the next command
    bciState = BCI_STANDBY;
}

void C_BCI_Package::createDebugDirectory()
{
    QDir dir(DEBUG_DIRECTORY);

    if (!dir.exists())
    {
        dir.mkdir(DEBUG_DIRECTORY);
    }
}

