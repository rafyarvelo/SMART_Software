#include "../bci_c_binary_parser.h"
#include "../bci_c_textparser.h"



int main(int argc, char** argv)
{
    QString inputfilename, outputfilename;
    C_BinaryParser binaryParser;
    C_TextParser   textParser;
    C_TM           tmData;

    //Get input/output filename from command line or use default
    if (argc < 3)
    {
        inputfilename  = DEFAULT_INPUT_NAME;
        outputfilename = DEFAULT_INPUT_NAME;
    }
    else
    {
        inputfilename = QString(argv[1]);
        outputfilename = QString(argv[2]);
    }

    //Parse through binary input data
    binaryParser.SetTMInputFilename(inputfilename);
    binaryParser.readTMData(inputfilename);

    //Output TM Data to a file
    textParser.writeTMData(tmData, outputfilename);


    return 0;
}
