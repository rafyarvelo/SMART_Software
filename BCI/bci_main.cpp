/*
 * bci_main.cpp
 *
 *  Created on: Jul 15, 2015
 *      Author: Rafy
 */
#include <QCoreApplication>
#include "bci_c_bci_package.h"

int main(int argc, char **argv)
{
    QCoreApplication a(argc, argv);
    C_BCI_Package* pBCIPackage = C_BCI_Package::Instance();

    //Lets Go!
    pBCIPackage->Run();

    return a.exec();
}
