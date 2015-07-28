/*
 * bci_c_bci_package.cpp
 *
 *  Created on: Jul 15, 2015
 *      Author: Rafy
 */

#include "bci_c_bci_package.h"

C_BCI_Package::C_BCI_Package()
{
	initialize();
	ofstream& bciLog = debugLog->BCI_Log();
		
	//Check EEG Connection Status...
	if (checkEEGComm())
	{
		bciLog << "EEG Communications Connected" << endl;
	}
	else
	{
		bciLog << "EEG Connection not created! Stopping BCI..." << endl;
		exit(1);
	}

	//Check PCC Connection Status...
	if (checkPCCComm())
	{
		bciLog << "PCC Communications Connected" << endl;
	}
	else
	{
		bciLog << "PCC Connection not created! Stopping BCI..." << endl;
		exit(1);
	}


	//Check BRSH Connection Status...
	if (checkBRSHComm())
	{
		bciLog << "BRSH Communications Connected" << endl;
	}
	else
	{
		bciLog << "BRSH Connection not created! Stopping BCI..." << endl;
		exit(1);
	}


	//Check EEG Connection Status...
	if (checkFlasherComm())
	{
		bciLog << "Flasher Communications Connected" << endl;
	}
	else
	{
		bciLog << "Flasher Connection not created! Stopping BCI..." << endl;
		exit(1);
	}
}

C_BCI_Package::~C_BCI_Package()
{
	
	if (pSignalProcessing)
		delete pSignalProcessing;
	if (pJA)
		delete pJA;
	if (pEEG_IO)
		delete pEEG_IO;
	if (pFlasherIO);
		delete pFlasherIO;
	if (pRVS);
		delete pRVS;
	if (pTM);
		delete pTM;
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
	
	if (pEEG_IO->GetConnectionStatus())
	{
		EEG_Data* eegData = pEEG_IO->getData();
		
		cout                << "READING DATA FROM EEG:" << endl;
		debugLog->BCI_Log() << "READING DATA FROM EEG:" << endl;
		for (int i = 0; i < eegData->size; i++)
		{
			sprintf(buffer, "Data[%d]: 0x%2x\n", i, eegData->rawData[i]);
			debugLog->BCI_Log() << buffer;
			cout                << buffer;
		}	
		cout                << "------------" << endl << endl;
		debugLog->BCI_Log() << "------------" << endl << endl;
	}
}

void C_BCI_Package::sendRVS_Commands()
{

}

void C_BCI_Package::sendPCC_Commands()
{

}

void C_BCI_Package::initialize()
{
	debugLog = SMART_DEBUG_LOG::Instance();
	debugLog->BCI_Log() << "Initializing BCI Package..." << endl;
	
	//Create Container Classes
	pRVS              = C_RVS::Instance();	
	pFlasherIO        = C_Flasher_IO::Instance(pRVS);	
	
	pEEG_IO           = createEEG_IO();
	pSignalProcessing = C_SignalProcessing::Instance(pEEG_IO); 
	pJA               = C_JudgmentAlgorithm::Instance(pSignalProcessing);
	
	pJA2BRS           = C_JA2BRS::Instance();
	pJA2PCC           = C_JA2PCC::Instance();
	
	pTM               = C_TM::Instance();
	
	debugLog->BCI_Log() << "TM Base Address at: " << pTM << endl;
	debugLog->BCI_Log() << "BCI Package Initialized Successfully" << endl;
}

bool C_BCI_Package::checkFlasherComm()
{
	return true;
}

bool C_BCI_Package::checkBRSHComm()
{
	return true;
}

bool C_BCI_Package::checkPCCComm()
{
	return true;
}

bool C_BCI_Package::checkEEGComm()
{
	return true;
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

