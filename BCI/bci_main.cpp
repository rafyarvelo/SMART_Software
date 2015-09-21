/*
 * bci_main.cpp
 *
 *  Created on: Jul 15, 2015
 *      Author: Rafy
 */

#include "bci_c_bci_package.h"

void ExtractTelemetry(QString inputfilename="", QString outputfilename="");

int main(int argc, char **argv)
{
    QCoreApplication a(argc, argv);
    C_BCI_Package* pBCIPackage = C_BCI_Package::Instance();

    //Lets Go!
    //pBCIPackage->Run();

    ExtractTelemetry();

    return 0;
}

void ExtractTelemetry(QString inputfilename, QString outputfilename)
{
    QString DEFAULT_OUTPUT_NAME = QString("../debug_files/extracted_telemetry.txt");
    QString DEFAULT_INPUT_NAME  = TM_DATA_OUTPUTFILE_BIN;
    C_BinaryParser binaryParser;
    C_TextParser   textParser;
    C_TM           tmData;

    //Get input/output filename from command line or use default
    if (inputfilename == "" || outputfilename == "")
    {
        inputfilename  = DEFAULT_INPUT_NAME;
        outputfilename = DEFAULT_OUTPUT_NAME;
    }

    //Parse through binary input data
    binaryParser.SetTMInputFilename(inputfilename);
    binaryParser.readTMData(inputfilename);

    //Output TM Data to a file
    textParser.writeTMData(tmData, outputfilename);
}
