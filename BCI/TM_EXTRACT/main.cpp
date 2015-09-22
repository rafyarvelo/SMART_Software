#include "../bci_c_binary_parser.h"
#include "../bci_c_textparser.h"

#define DEFAULT_INPUT_NAME  QString("../../debug_files/tm_output_data.bin")
#define DEFAULT_OUTPUT_NAME QString("../../debug_files/extracted_telemetry.txt")

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
        outputfilename = DEFAULT_OUTPUT_NAME;
    }
    else
    {
        inputfilename = QString(argv[1]);
        outputfilename = QString(argv[2]);
    }

    //Parse through binary input data
    cout << "Reading TM from inputfile: " << inputfilename.toStdString() << endl;
    binaryParser.SetTMInputFilename(inputfilename);
    binaryParser.readTMData(inputfilename);

    //Output TM Data to a file
    cout << "Outputting TM to File: " << outputfilename.toStdString() << endl;
    textParser.writeTMData(tmData, outputfilename);

    return 0;
}
