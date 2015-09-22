#include "bci_c_textparser.h"



C_TextParser::C_TextParser(const QString& filename, QIODevice::OpenModeFlag openMode)
    : C_AbstractParser(filename, openMode)
{
    stream.setDevice(this->fp);
    delimeter = ","; //Default Delimeter
}

C_TextParser::~C_TextParser()
{

}

//Read EEG Data
C_EEG_Data& C_TextParser::readEEGData ()
{

}

//Write EEG Data
void C_TextParser::writeEEGData()
{
    writeEEGHeader();

    for (int i = 0; i < eegData.size(); i++)
    {
        writeEEGFrame(eegData.GetFramePtr(i));
        stream << endl;
    }
}

//Read BRS Data
C_TM& C_TextParser::readTMData ()
{
    return tmData;
}

//Write BRS Data
void C_TextParser::writeTMData()
{
    for (int i = 0; i < tmData.size(); i++)
    {
        writeTMFrame(tmData.GetFrame(i));
    }
}

EEG_Frame_t* C_TextParser::readEEGFrame()
{
    EEG_Frame_t* buffer = EEG_Frame_t::create();

    return buffer;
}

TM_Frame_t*  C_TextParser::readTMFrame()
{
    TM_Frame_t* buffer = TM_Frame_t::createFrame();

    return buffer;
}

void C_TextParser::writeEEGFrame(EEG_Frame_t* frame)
{
    int i = 0;
    stream << frame->eegType << delimeter;
    stream << frame->counter << delimeter;

    //Electrode Data, Ignore Spares
    for (i = 0; i < MAX_EEG_ELECTRODES; i++)
    {
        stream << frame->electrodeData[i] << delimeter;
    }

    //Contact Quality, Ignore Spares
    for (i = 0; i < MAX_EEG_ELECTRODES; i++)
    {
        stream << frame->contactQuality[i] << delimeter;
    }

    //The Rest of the Crap
    stream << frame->gyroX             << delimeter;
    stream << frame->gyroY             << delimeter;
    stream << frame->batteryPercentage << delimeter;
}

void C_TextParser::writeTMFrame(TM_Frame_t* frame)
{
    static bool firstFrame = true;

    if (firstFrame)
    {
        writeTMHeader();
        firstFrame = false;
    }

    stream << frame->timeStamp << delimeter;
    writeEEGFrame(&frame->eegFrame);
    writeBRSFrame(&frame->brsFrame);
    stream << frame->ledForward .frequency   << delimeter;
    stream << frame->ledBackward.frequency   << delimeter;
    stream << frame->ledRight   .frequency   << delimeter;
    stream << frame->ledLeft    .frequency   << delimeter;
    stream << frame->eegConnectionStatus     << delimeter;
    stream << frame->pccConnectionStatus     << delimeter;
    stream << frame->brsConnectionStatus     << delimeter;
    stream << frame->flasherConnectionStatus << delimeter;

    stream << endl;
}

void C_TextParser::writeBRSFrame(BRS_Frame_t* frame)
{
    stream << frame->gpsData.altitude     << delimeter;
    stream << frame->gpsData.longitude    << delimeter;
    stream << frame->gpsData.altitude     << delimeter;
    stream << frame->gpsData.groundSpeed  << delimeter;
    stream << frame->usData.rangeToObject << delimeter;
    stream << (char) frame->remoteCommand << delimeter;
}

void C_TextParser::writeTMHeader()
{
    for (int i = 0; i < headerNames.tmFrameNames.size(); i++)
    {
        stream << headerNames.tmFrameNames.at(i) << delimeter;
    }
    stream << endl;
}

void C_TextParser::writeEEGHeader()
{
    for (int i = 0; i < headerNames.eegFrameNames.size(); i++)
    {
        stream << headerNames.eegFrameNames.at(i) << delimeter;
    }
    stream << endl;
}
