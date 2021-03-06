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

C_BinaryParser::C_BinaryParser(const QString& filename, QIODevice::OpenModeFlag openMode)
    : C_AbstractParser(filename, openMode)
{
    stream.setByteOrder(QDataStream::LittleEndian);
    stream.setDevice(this->fp);
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
    stream.writeRawData(data, size);
}

EEG_Frame_t* C_BinaryParser::readEEGFrame()
{
    EEG_Frame_t* frame = 0;

    //Find the Start of the EEG Frame
    if (findSync(EEG_FRAME_START))
    {
        //Read Data
        frame = new EEG_Frame_t;
        stream.readRawData(reinterpret_cast<char*> (frame), sizeof(EEG_Frame_t));
    }

    return frame;
}

TM_Frame_t*  C_BinaryParser::readTMFrame()
{
    TM_Frame_t* frame = 0;

    //Find the Start of the TM Frame
    if (findSync(TM_FRAME_START))
    {
        //Read Data
        frame = new TM_Frame_t;
        stream.readRawData(reinterpret_cast<char*> (frame), sizeof(TM_Frame_t));
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
    while (!stream.atEnd())
    {
        stream.readRawData((char*) &temp, sizeof(TelemetrySyncType));

        if (sync == temp)
        {
            return true;
        }
    }

    return false;
}
