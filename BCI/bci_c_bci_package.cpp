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
    if (tmDataBuffer)
    {
        delete tmDataBuffer;
    }
    if (eegDataBuffer)
    {
        delete eegDataBuffer;
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
	EEG_Data* eegData = 0;
	char buffer[100];
	
    if (!pEEG_IO->connect())
	{
        debugLog->BCI_Log() << "Lost Connection to EEG!" << cout;
        eegConnectionStatus = NOT_CONNECTED;
        return;
    }
    else
    {
        eegConnectionStatus = CONNECTED;
    }

    //cout                << "READING DATA FROM EEG:" << endl;
    debugLog->BCI_Log() << "READING DATA FROM EEG:" << endl;
    eegData = pEEG_IO->getData();

    for (sizeType i = 0; i < eegData->size; i++)
    {
        sprintf(buffer, "Data[%d]: 0x%2x\n", i, eegData->rawData[i]);
        debugLog->BCI_Log() << buffer;
        //cout                << buffer;
    }
   // cout                << "------------" << endl << endl;
    debugLog->BCI_Log() << "------------" << endl << endl;
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
		case EEG_TYPE_EMOTIV:
			ptr = C_EEG_IO_EMOTIV::Instance();
			break;
		case EEG_TYPE_NAUTILUS:
			ptr = C_EEG_IO_NAUTILUS::Instance();
			break;
		default:
			ptr = C_EEG_IO_DEBUG::Instance();
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
        cout   << "EEG Disconnected! Stopping BCI..." << endl;
        status = NOT_CONNECTED;
    }

    //Check PCC Connection Status...
    if (!pccConnectionStatus)
    {
        cout   << "PCC Disconnected! Stopping BCI..." << endl;
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
            bciState = BCI_INITIALIZATION;//Start initializing!
            break;
        case BCI_INITIALIZATION:

            //Make Connections
            initialize();

            //Configure Repetitive Visual Stimulus and send to Flasher
            pRVS->Generate();
            pJA->SetRVS(pRVS);
            pFlasherIO->SendRVS();

            startEEG();
            debugLog->BCI_Log() << "Initialization Complete, Moving to STANBY..." << endl;

            stopwatch.start(); //Check for a timeout
            bciState = BCI_STANDBY;
            break;
        case BCI_STANDBY:
            //Wait for an interrupt or a timeout
            if (stopwatch.elapsed() > COMMAND_TIMEOUT)
            {
                debugLog->BCI_Log() << "Timed out waiting for EEG Data: Count = " << ++missCount << endl;
                stopwatch.restart();
            }

            if (missCount < MAX_MISSES)
            {
                debugLog->BCI_Log() << "Resending last command due to timeout..." << endl;
                //Hack for Now
                pPCC_IO->SetCommand(PCC_FORWARD);
                pPCC_IO->SendCommand();
            }
            else
            {
                debugLog->BCI_Log() << "Maximum Miss Count Reached." << endl;
                cout << "Connection to EEG Timed out" << endl;
                exit(1);
            }

            break;
        case BCI_PROCESSING:
            pJA->SetTM(pBRS_IO->GetLatestTM());
            pJA->computeCommand();

            break;
        case BCI_READY:
            pPCC_IO->SetCommand(pJA->GetFinalCommand());
            pPCC_IO->SendCommand();

            pBRS_IO->SendTM();
            bciState = BCI_STANDBY;
            break;
        }
    }

    //Should Never Get to this line
    cout << "ERROR! BCI Lost Connection!" << endl;
}
