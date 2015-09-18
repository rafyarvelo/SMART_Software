#include "bci_c_binary_parser.h"

//Static Constants
const TelemetrySyncType C_BinaryParser::TM_DATA_START   = 0x12345678;
const TelemetrySyncType C_BinaryParser::TM_DATA_END     = 0x87654321;
const TelemetrySyncType C_BinaryParser::EEG_DATA_START  = 0x89ABCDEF;
const TelemetrySyncType C_BinaryParser::EEG_DATA_STOP   = 0xFEDCBA98;
const TelemetrySyncType C_BinaryParser::EEG_FRAME_START = 0x3456789A;
const TelemetrySyncType C_BinaryParser::EEG_FRAME_STOP  = 0xA9876543;
const TelemetrySyncType C_BinaryParser::TM_FRAME_START  = 0x456789AB;
const TelemetrySyncType C_BinaryParser::TM_FRAME_END    = 0xBA987654;

//Static Flags
bool C_BinaryParser::eegDataStarted = false;
bool C_BinaryParser::tmDataStarted  = false;

C_BinaryParser::C_BinaryParser()
{

    //Initialize Default Input/Output Files
    SetTMInputFilename(TM_DATA_INPUTFILE_BIN);
    SetTMOutputFilename(TM_DATA_OUTPUTFILE_BIN);
    SetEEGInputFilename(EEG_DATA_INPUTFILE_BIN);
    SetEEGOutputFilename(EEG_DATA_OUTPUTFILE_BIN);
}

C_BinaryParser::~C_BinaryParser()
{
    //Write the End syncs before destruction
    if (eegDataStarted)
    {
        writeRawData((char*) &EEG_DATA_STOP, sizeof(TelemetrySyncType));
    }

    if (tmDataStarted)
    {
        writeRawData((char*) &TM_DATA_END, sizeof(TelemetrySyncType));
    }
}

void C_BinaryParser::writeRawData(char *data, sizeType size)
{
    dataOut.writeBytes(data, size);
}

//Read EEG Data
C_EEG_Data& C_BinaryParser::readEEGData (const QString& filename)
{
    //Clear Existing Data
    eegData.clear();

    //Set EEG Filename
    SetEEGInputFilename(filename);
    dataIn.setDevice(eegDataIn);

    while (!dataIn.atEnd())
    {
        eegData.AddFrame(readEEGFrame());
    }

    return eegData;
}

//Write EEG Data
void C_BinaryParser::writeEEGData(const QString& filename)
{
    //Set EEG Filename
    SetEEGOutputFilename(filename);
    dataOut.setDevice(eegDataOut);

    //Tell the other functions that we already wrote the start sync
    eegDataStarted = true;

    //Write the Data enclosed in Syncs
    writeRawData((char*) EEG_DATA_START, sizeof(TelemetrySyncType));
    for (int i = 0; i < eegData.size(); i++)
    {
        writeEEGFrame(eegData.GetFramePtr(i));
    }
    writeRawData((char*) EEG_DATA_STOP, sizeof(TelemetrySyncType));
}

//Read BRS Data
C_TM& C_BinaryParser::readTMData (const QString& filename)
{
    //Clear Existing Data
    tmData.clear();

    //Set EEG Filename
    SetTMInputFilename(filename);
    dataIn.setDevice(tmDataIn);

    while (!dataIn.atEnd())
    {
        tmData.addFrame(readTMFrame());
    }

    return tmData;
}

//Write BRS Data
void C_BinaryParser::writeTMData(const QString& filename)
{
    //Set TM Filename
    SetTMOutputFilename(filename);
    dataOut.setDevice(tmDataOut);

    //Tell the other functions that we already wrote the start sync
    tmDataStarted = true;

    //Write the Data enclosed in Syncs
    writeRawData((char*) TM_DATA_START, sizeof(TelemetrySyncType));
    for (int i = 0; i < eegData.size(); i++)
    {
        writeTMFrame(tmData.GetFrame(i));
    }
    writeRawData((char*) TM_DATA_END, sizeof(TelemetrySyncType));
}

void C_BinaryParser::writeEEGData(C_EEG_Data& data, const QString& filename)
{
    C_AbstractParser::writeEEGData(data, filename);
}

void C_BinaryParser::writeTMData(C_TM& data, const QString& filename)
{
    C_AbstractParser::writeTMData(data, filename);
}

EEG_Frame_t* C_BinaryParser::readEEGFrame()
{
    EEG_Frame_t* frame = EEG_Frame_t::create();

    //Find the Start of the EEG Frame
    if (findSync(EEG_FRAME_START))
    {
        //Read Data
        dataIn.readRawData(reinterpret_cast<char*> (frame), sizeof(EEG_Frame_t));
    }

    return frame;
}

TM_Frame_t*  C_BinaryParser::readTMFrame()
{
    TM_Frame_t* frame = TM_Frame_t::createFrame();

    //Find the Start of the TM Frame
    if (findSync(TM_FRAME_START))
    {
        //Read Data
        dataIn.readRawData(reinterpret_cast<char*> (frame), sizeof(TM_Frame_t));
    }

    return frame;
}

void C_BinaryParser::writeEEGFrame(EEG_Frame_t* frame)
{
    //Write the Start Sync if necessary
    if (!eegDataStarted)
    {
        writeRawData((char*) &EEG_DATA_START, sizeof(TelemetrySyncType));
        eegDataStarted = true;
    }

    //Write the Frame enclosed in syncs
    writeRawData((char*) &EEG_FRAME_START, sizeof(TelemetrySyncType));
    writeRawData(reinterpret_cast<char*>(frame), sizeof(EEG_Frame_t));
    writeRawData((char*) &EEG_FRAME_STOP, sizeof(TelemetrySyncType));
}

void C_BinaryParser::writeTMFrame(TM_Frame_t* frame)
{
    //Write the Start Sync if necessary
    if (!tmDataStarted)
    {
        writeRawData((char*) &TM_DATA_START, sizeof(TelemetrySyncType));
        tmDataStarted = true;
    }

    //Write the Frame enclosed in syncs
    writeRawData((char*) &TM_FRAME_START, sizeof(TelemetrySyncType));
    writeRawData(reinterpret_cast<char*>(frame), sizeof(TM_Frame_t));
    writeRawData((char*) &TM_FRAME_END, sizeof(TelemetrySyncType));
}

//Look For a Sync, Return True if Found
bool C_BinaryParser::findSync(TelemetrySyncType sync)
{
    TelemetrySyncType temp;

    //Find the Start of the EEG Frame
    while (!dataIn.atEnd())
    {
        dataIn.readRawData((char*) &temp, sizeof(TelemetrySyncType));

        if (sync == temp)
        {
            return true;
        }
    }

    return false;
}
