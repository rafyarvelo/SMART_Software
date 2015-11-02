#include "bci_c_binary_parser.h"
#include "bci_c_textparser.h"

#define DEFAULT_TM_INPUT_NAME  QString("../../debug_files/tm_output_data.bin")
#define DEFAULT_TM_OUTPUT_NAME QString("../../debug_files/extracted_telemetry.csv")

#define DEFAULT_EEG_INPUT_NAME  QString("../../debug_files/eeg_output_data.bin")
#define DEFAULT_EEG_OUTPUT_NAME QString("../../debug_files/extracted_eeg_telemetry.csv")

int main(int argc, char** argv)
{
    QString tmInFile, tmOutFile;
    QString eegInFile, eegOutFile;

    C_BinaryParser* binaryParser = 0;
    C_TextParser*   textParser   = 0;
    TM_Frame_t*     tmFrame      = 0;
    EEG_Frame_t*    eegFrame     = 0;
    unsigned int cnt = 0;

    cout <<"TM FRAME SIZE: " << sizeof(TM_Frame_t) << endl;
    //Get input/output filename from command line or use default
    if (argc < 5)
    {
        tmInFile   = DEFAULT_TM_INPUT_NAME;
        tmOutFile  = DEFAULT_TM_OUTPUT_NAME;
        eegInFile  = DEFAULT_EEG_INPUT_NAME;
        eegOutFile = DEFAULT_EEG_OUTPUT_NAME;
    }
    else
    {
        tmInFile   = QString(argv[1]);
        tmOutFile  = QString(argv[2]);
        eegInFile  = QString(argv[3]);
        eegOutFile = QString(argv[4]);
    }

    //Extract TM Data
    binaryParser = new C_BinaryParser(tmInFile, QIODevice::ReadOnly);
    textParser   = new C_TextParser(tmOutFile, QIODevice::WriteOnly);

    cout << "Reading TM from inputfile: " << tmInFile.toStdString() << endl;
    cout << "Outputting TM to File: "     << tmOutFile.toStdString() << endl;

    tmFrame = binaryParser->readTMFrame();

    //Write all of the Frames in the File
    while (tmFrame)
    {
        textParser->writeTMFrame(tmFrame);
        tmFrame = binaryParser->readTMFrame();
        cnt++;
    }
    cout << "TM Frames Found: " << cnt << endl;

    //Clean up
    delete binaryParser;
    delete textParser;
    cnt = 0;
    cout << endl;

    //Extract EEG Data
    binaryParser = new C_BinaryParser(eegInFile, QIODevice::ReadOnly);
    textParser   = new C_TextParser(eegOutFile, QIODevice::WriteOnly);

    cout <<"EEG FRAME SIZE: " << sizeof(EEG_Frame_t) << endl;
    cout << "Reading EEG TM from inputfile: " << eegInFile.toStdString() << endl;
    cout << "Outputting EEG TM to File: "     << eegOutFile.toStdString() << endl;

    eegFrame = binaryParser->readEEGFrame();

    //Write all of the Frames in the File
    while (eegFrame)
    {
        textParser->writeEEGFrame(eegFrame);
        eegFrame = binaryParser->readEEGFrame();
        cnt++;
    }
    cout << "EEG Frames Found: " << cnt << endl;


    return 0;
}
