#ifndef BCI_C_BINARY_PARSER_H
#define BCI_C_BINARY_PARSER_H

#include <QDataStream>
#include "bci_c_abstract_parser.h"

//Default Input/Output Filenames
#ifdef WIN32
    const QString TM_DATA_INPUTFILE_BIN   = ".\\debug_files\\tm_input_data.bin";
    const QString TM_DATA_OUTPUTFILE_BIN  = ".\\debug_files\\tm_output_data.bin";
    const QString EEG_DATA_INPUTFILE_BIN  = ".\\debug_files\\eeg_input_data.bin";
    const QString EEG_DATA_OUTPUTFILE_BIN = ".\\debug_files\\eeg_output_data.bin";
#else
    const QString TM_DATA_INPUTFILE_BIN   = "./debug_files/tm_input_data.bin";
    const QString TM_DATA_OUTPUTFILE_BIN  = "./debug_files/tm_output_data.bin";
    const QString EEG_DATA_INPUTFILE_BIN  = "./debug_files/eeg_input_data.bin";
    const QString EEG_DATA_OUTPUTFILE_BIN = "./debug_files/eeg_output_data.bin";
#endif

typedef quint32 TelemetrySyncType;

class C_BinaryParser : public C_AbstractParser
{
public:
    C_BinaryParser(const QString& filename, QIODevice::OpenModeFlag openMode);
    virtual ~C_BinaryParser();

    //Read/Write EEG Data
    virtual C_EEG_Data& readEEGData ();
    virtual void writeEEGData();
    virtual void writeTMData(C_EEG_Data& data) {eegData = data; writeEEGData();}

    //Read/Write BRS Data
    virtual C_TM& readTMData ();
    virtual void writeTMData();
    virtual void writeTMData(C_TM& data) {tmData = data; writeTMData();}

    //Read Individual Frames
    virtual EEG_Frame_t* readEEGFrame();
    virtual TM_Frame_t*  readTMFrame();
    virtual void writeEEGFrame(EEG_Frame_t* frame);
    virtual void writeTMFrame(TM_Frame_t* frame);

    void writeRawData(char* data, sizeType size);
    bool findSync(TelemetrySyncType sync);

    //Syncs to Make sure we're reading the right data
    static const TelemetrySyncType TM_DATA_START;
    static const TelemetrySyncType TM_DATA_END;
    static const TelemetrySyncType EEG_DATA_START;
    static const TelemetrySyncType EEG_DATA_STOP;
    static const TelemetrySyncType EEG_FRAME_START;
    static const TelemetrySyncType EEG_FRAME_STOP;
    static const TelemetrySyncType TM_FRAME_START;
    static const TelemetrySyncType TM_FRAME_END;

    //Binary I/O Api
private:
    QDataStream stream;

    static bool eegDataStarted;
    static bool tmDataStarted;
};

#endif // BCI_C_BINARY_PARSER_H
