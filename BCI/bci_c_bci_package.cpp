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

    //Create Container Classes
    pRVS              = C_RVS::Instance();
    pFlasherIO        = C_Flasher_IO::Instance(pRVS);

    pEEG_IO           = createEEG_IO();
    pSignalProcessing = C_SignalProcessing::Instance(pEEG_IO);
    pJA               = C_JudgmentAlgorithm::Instance(pSignalProcessing);

    pBRS_IO           = C_BRSH_IO::Instance();
    pPCC_IO           = C_PCC_IO::Instance();

    eegConnectionStatus      = NOT_CONNECTED;
    flasherConnectionStatus  = NOT_CONNECTED;
    pccConnectionStatus      = NOT_CONNECTED;
    brshConnectionStatus     = NOT_CONNECTED;

    debugLog->BCI_Log() << "BCI Package Instantiated Successfully" << endl;
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

C_BCI_Package* C_BCI_Package::Instance()
{
	C_BCI_Package* ptr = new C_BCI_Package();

	if (!ptr)
	{
		cout << "C_BCI_Package Instance could not be created!" << endl;
	}
	else
	{
		cout << "BCI Package Instance Created at: " << ptr << endl;
	}
	return ptr;
}


void C_BCI_Package::startEEG()
{
    pEEG_IO->start();
}

void C_BCI_Package::initialize()
{
    debugLog->BCI_Log() << "Connecting to Flasher..." << endl;
    flasherConnectionStatus = pFlasherIO->connect();

    debugLog->BCI_Log() << "Connecting to EEG..." << endl;
    eegConnectionStatus = pEEG_IO->connect();

    debugLog->BCI_Log() << "Connecting to BRSH..." << endl;
    brshConnectionStatus = pBRS_IO->connect();

    debugLog->BCI_Log() << "Connecting to PCC..." << endl;
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
        debugLog->BCI_Log() << "EEG Connection not created! Stopping BCI..." << endl;
        cerr   << "EEG Disconnected! Stopping BCI..." << endl;
        status = NOT_CONNECTED;
    }

    //Check PCC Connection Status...
    if (!pccConnectionStatus)
    {
        cerr   << "PCC Disconnected! Stopping BCI..." << endl;
        status = NOT_CONNECTED;
    }


    //Check BRSH Connection Status...
    if (!brshConnectionStatus)
    {
        debugLog->BCI_Log() << "BRSH Disconnected! Stopping BCI..." << endl;
        status = NOT_CONNECTED;
    }

    //Check EEG Connection Status...
    if (!flasherConnectionStatus)
    {
        debugLog->BCI_Log() << "Flasher Disconnected! Stopping BCI..." << endl;
        status = NOT_CONNECTED;
    }

    return status;
}

void C_BCI_Package::Run()
{
    while (checkConnections())
    {
        switch (bciState)
        {
        case BCI_OFF:
            bciState = BCI_INITIALIZATION; //Start initializing!
            break;

        case BCI_INITIALIZATION:

            //Make Connections
            initialize();

            //Configure Repetitive Visual Stimulus and send to Flasher
            pRVS->Generate();
            pJA->SetRVS(pRVS);
            pFlasherIO->SendRVS();

            startEEG();


            debugLog->BCI_Log() << "Initialization Complete, Moving to STANDBY..." << endl;
            bciState = BCI_STANDBY;
            break;

        case BCI_STANDBY:
            //Keep Track of the Mission Time
            stopwatch.start();

            //Manage Telemetry Stream
            updateTM();

            break;

        case BCI_PROCESSING:
            //Update the Judgment Algorithm with the current data
            pJA->SetTM(currentTMFrame);

            //Decide Final Power Chair Command
            pJA->computeCommand();

            //Move to Ready to send the command
            bciState = BCI_READY;
            break;

        case BCI_READY:
            //Send the PCC Command then Revert to STANDBY
            pPCC_IO->SetCommand(pJA->GetFinalCommand());
            pPCC_IO->SendCommand();

            bciState = BCI_STANDBY;
            break;
        }
    }

    cerr << "ERROR! BCI Lost Connection!" << endl;
}

void C_BCI_Package::updateTM()
{
    if (!currentTMFrame)
    {
        currentTMFrame = TM_Frame_t::createFrame();
    }

    //Update EEG, BRS, and Flasher Data
    currentTMFrame->eegFrame  = eegData.GetFrame();
    currentTMFrame->brsFrame  = pBRS_IO->GetLatestBRSFrame();
    currentTMFrame->ledGroups = pRVS->GetAllLEDGroups();

    //Update Connection Status
    currentTMFrame->eegConnectionStatus = eegConnectionStatus;
    currentTMFrame->pccConnectionStatus = pccConnectionStatus;
    currentTMFrame->brsConnectionStatus = brshConnectionStatus;
    currentTMFrame->flasherConnectionStatus = flasherConnectionStatus;
}

//Slots
void C_BCI_Package::onEEGDataProcessed(C_EEG_Data &eegData)
{
    //If we're not in standby, then just ignore the EEG
    if (bciState == BCI_STANDBY)
    {
        //Update our EEG Data with the Latest Data
        this->eegData = eegData;

        //Update Telemetry and Process Command
        updateTM();
        bciState = BCI_PROCESSING;
    }
}

void C_BCI_Package::onRemoteCmdReceived(PCC_Command_Type& cmd)
{
    //If we're not in standby, then just ignore the Cmd
    if (bciState == BCI_STANDBY)
    {
        //Update TM with the command, then let the
        //Judgment Algorithm take care of the rest
        updateTM();
        currentTMFrame->brsFrame.remoteCommand = cmd;

        bciState = BCI_PROCESSING;
    }
}

void C_BCI_Package::onEmergencyStopRequested()
{
    //No messing around here, just send the command
    pPCC_IO->SetCommand(PCC_STOP);
    pPCC_IO->SendCommand();
}
