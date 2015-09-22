/*
 * bci_main.cpp
 *
 *  Created on: Jul 15, 2015
 *      Author: Rafy
 */

#include "bci_c_bci_package.h"

int main(int argc, char **argv)
{
//     QCoreApplication a(argc, argv);
//     C_BCI_Package* pBCIPackage = C_BCI_Package::Instance();

//    //Lets Go!
//    pBCIPackage->Run();

//    return a.exec();


    QString in("../debug_files/TEST.bin");
    QString out("../debug_files/TEST_OUT.txt");
    C_BinaryParser binaryParser(in, WRITE);
    C_BinaryParser bin(in, READ);
    C_TextParser   textParser(out, WRITE);

    C_TM tmData;
    int i;

    for (i = 0; i < 10; i++)
    {
        tmData.addFrame(TM_Frame_t::createFrame());
    }

    for(i = 0; i < tmData.size(); i++)
    {
        binaryParser.writeTMFrame(tmData.GetFrame(i));
    }

    textParser.writeTMData(tmData);
}
