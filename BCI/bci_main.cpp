/*
 * bci_main.cpp
 *
 *  Created on: Jul 15, 2015
 *      Author: Rafy
 */

#include "bci_c_bci_package.h"

int main(void)
{

    C_BCI_Package* pBCIPackage = C_BCI_Package::Instance();
    pBCIPackage->Run();

	return 0;
}
