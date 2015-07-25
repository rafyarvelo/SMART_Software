/*
 * bci_main.cpp
 *
 *  Created on: Jul 15, 2015
 *      Author: Rafy
 */

#include "bci_c_bci_package.h"

int main(void)
{
	SMART_DEBUG_LOG* debugLog = SMART_DEBUG_LOG::Instance();
	C_BCI_Package* pBCIPackage = C_BCI_Package::Instance();
	
	debugLog->BCI_Log() << "BCI Package created at: " << pBCIPackage << endl;
		
	for (int i = 0; i < 10; i ++)
	{
		pBCIPackage->startEEG();//TEST TEST TEST
	}
	return 0;
}
