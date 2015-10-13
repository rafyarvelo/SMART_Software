#include "bci_c_binary_parser.h"
#include "bci_c_textparser.h"

#define DEFAULT_INPUT_NAME  QString("tm_output_data.bin")
#define DEFAULT_OUTPUT_NAME QString("extracted_telemetry.csv")

int main(int argc, char** argv)
{
    QString inputfilename, outputfilename;
    C_BinaryParser* binaryParser = 0;
    C_TextParser*   textParser   = 0;
    TM_Frame_t*     frame        = 0;

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

    binaryParser = new C_BinaryParser(inputfilename, QIODevice::ReadOnly);
    textParser   = new C_TextParser(outputfilename, QIODevice::WriteOnly);

    cout << "Reading TM from inputfile: " << inputfilename.toStdString() << endl;
    cout << "Outputting TM to File: "     << outputfilename.toStdString() << endl;

    frame = createTMFrame();
    frame->timeStamp = 1; //to get into the while loop

    //Write all of the Frames in the File
    while (frame && frame->timeStamp)
    {
        frame = binaryParser->readTMFrame();
        textParser->writeTMFrame(frame);
    }

    return 0;
}
