#ifndef BCI_C_TEXTPARSER_H
#define BCI_C_TEXTPARSER_H

#include <QTextStream>
#include "bci_c_abstract_parser.h"
#include "bci_c_tm_header_names.h"

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

    //Set Delimeter for output files
    void SetDelimeter(const QString& delim) { delimeter = delim; }
    void SetDelimeter(const char* delim = ",") { SetDelimeter(QString(delim));}

private:
    void writeBRSFrame(BRS_Frame_t* frame);
    void writeTMHeader();
    void writeEEGHeader();

private:
    QTextStream dataIn;
    QTextStream dataOut;

    C_HEADER_NAMES headerNames;
    QString delimeter;
};

#endif // BCI_C_TEXTPARSER_H
