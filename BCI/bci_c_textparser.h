#ifndef BCI_C_TEXTPARSER_H
#define BCI_C_TEXTPARSER_H

#include <QTextStream>
#include "bci_c_abstract_parser.h"
#include "bci_c_tm_header_names.h"

//Default Input/Output Filenames
#ifdef WIN32
    const QString TM_DATA_INPUTFILE_TXT   = ".\\debug_files\\tm_input_data.txt";
    const QString TM_DATA_OUTPUTFILE_TXT  = ".\\debug_files\\tm_output_data.txt";
    const QString EEG_DATA_INPUTFILE_TXT  = ".\\debug_files\\eeg_input_data.txt";
    const QString EEG_DATA_OUTPUTFILE_TXT = ".\\debug_files\\eeg_output_data.txt";
#else
    const QString TM_DATA_INPUTFILE_TXT   = "./debug_files/tm_input_data.txt";
    const QString TM_DATA_OUTPUTFILE_TXT  = "./debug_files/tm_output_data.txt";
    const QString EEG_DATA_INPUTFILE_TXT  = "./debug_files/eeg_input_data.txt";
    const QString EEG_DATA_OUTPUTFILE_TXT = "./debug_files/eeg_output_data.txt";
#endif

class C_TextParser : public C_AbstractParser
{
public:
    C_TextParser(const QString& filename, QIODevice::OpenModeFlag openMode);
    virtual ~C_TextParser();

    //Read/Write EEG Data
    virtual C_EEG_Data& readEEGData ();
    virtual void writeEEGData();
    virtual void writeEEGData(C_EEG_Data& data) {eegData = data; writeEEGData();}

    //Read/Write BRS Data
    virtual C_TM& readTMData ();
    virtual void writeTMData();
    virtual void writeTMData(C_TM& data) {tmData = data; writeTMData();}

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
    QTextStream stream;

    C_HEADER_NAMES headerNames;
    QString delimeter;
};

#endif // BCI_C_TEXTPARSER_H
