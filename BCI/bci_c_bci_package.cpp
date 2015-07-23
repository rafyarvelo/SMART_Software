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

	//Check EEG Connection Status...
	if (checkEEGComm())
	{
		cout << "EEG Communications Connected" << endl;
	}
	else
	{
		cout << "EEG Connection not created! Stopping BCI..." << endl;
		exit(1);
	}

	//Check PCC Connection Status...
	if (checkPCCComm())
	{
		cout << "PCC Communications Connected" << endl;
	}
	else
	{
		cout << "PCC Connection not created! Stopping BCI..." << endl;
		exit(1);
	}


	//Check BRSH Connection Status...
	if (checkBRSHComm())
	{
		cout << "BRSH Communications Connected" << endl;
	}
	else
	{
		cout << "BRSH Connection not created! Stopping BCI..." << endl;
		exit(1);
	}


	//Check EEG Connection Status...
	if (checkFlasherComm())
	{
		cout << "Flasher Communications Connected" << endl;
	}
	else
	{
		cout << "Flasher Connection not created! Stopping BCI..." << endl;
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
	C_BCI_Package* ptr = new C_BCI_Package;

	if (!ptr)
	{
		cout << "C_BCI_Package Instance could not be created!" << endl;
	}
	else
	{
		cout << "BCI Package Initialized" << endl;
	}
	return ptr;
}


void C_BCI_Package::startEEG()
{

}

void C_BCI_Package::sendRVS_Commands()
{

}

void C_BCI_Package::sendPCC_Commands()
{

}

void C_BCI_Package::initialize()
{
	cout << "Initializing BCI Package..." << endl;
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



