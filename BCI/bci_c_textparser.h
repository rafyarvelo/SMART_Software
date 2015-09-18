#ifndef BCI_C_TEXTPARSER_H
#define BCI_C_TEXTPARSER_H

#include <QTextStream>
#include "bci_c_abstract_parser.h"

//Default Input/Output Filenames
const QString TM_DATA_INPUTFILE_TXT  = "../debug_files/brs_input_data.txt";
const QString TM_DATA_OUTPUTFILE_TXT = "../debug_files/brs_output_data.txt";

const QString EEG_DATA_INPUTFILE_TXT  = "../debug_files/eeg_input_data.txt";
const QString EEG_DATA_OUTPUTFILE_TXT = "../debug_files/eeg_output_data.txt";

class C_TextParser : public C_AbstractParser
{
public:
    C_TextParser();
    virtual ~C_TextParser();

    //Read/Write EEG Data
    virtual C_EEG_Data& readEEGData (const QString& filename = EEG_DATA_INPUTFILE_TXT);
    virtual void writeEEGData(const QString& filename = EEG_DATA_OUTPUTFILE_TXT);
    void writeEEGData(C_EEG_Data& data, const QString& filename = EEG_DATA_OUTPUTFILE_TXT);

    //Read/Write BRS Data
    virtual C_TM& readTMData (const QString& filename = TM_DATA_INPUTFILE_TXT);
    virtual void writeTMData(const QString& filename = TM_DATA_OUTPUTFILE_TXT);
    void writeTMData(C_TM& data, const QString& filename = TM_DATA_OUTPUTFILE_TXT);

    //Read Individual Frames
    virtual EEG_Frame_t* readEEGFrame();
    virtual TM_Frame_t*  readTMFrame();
    virtual void writeEEGFrame(EEG_Frame_t* frame);
    virtual void writeTMFrame(TM_Frame_t* frame);

    //Text Filea I/O Api
private:
    QTextStream dataIn;
    QTextStream dataOut;
};

#endif // BCI_C_TEXTPARSER_H
