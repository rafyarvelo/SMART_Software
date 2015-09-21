#include "bci_c_textparser.h"



C_TextParser::C_TextParser()
{
    //Initialize Default Input/Output Files
    SetTMInputFilename(TM_DATA_INPUTFILE_TXT);
    SetTMOutputFilename(TM_DATA_OUTPUTFILE_TXT);
    SetEEGInputFilename(EEG_DATA_INPUTFILE_TXT);
    SetEEGOutputFilename(EEG_DATA_OUTPUTFILE_TXT);
}

C_TextParser::~C_TextParser()
{

}

//Read EEG Data
C_EEG_Data& C_TextParser::readEEGData (const QString& filename)
{
    //Set EEG Filename
    SetEEGInputFilename(filename);

}

//Write EEG Data
void C_TextParser::writeEEGData(const QString& filename)
{
    //Set EEG Filename
    SetEEGOutputFilename(filename);

}

//Read BRS Data
C_TM& C_TextParser::readTMData (const QString& filename)
{
    //Set TM Filename
    SetTMInputFilename(filename);

    return tmData;
}

//Write BRS Data
void C_TextParser::writeTMData(const QString& filename)
{
    //Set TM Filename
    SetTMOutputFilename(filename);
    dataOut.setDevice(tmDataOut);

    for (int i = 0; i < tmData.size(); i++)
    {
        writeTMFrame(tmData.GetFrame(i));
    }
}

void C_TextParser::writeEEGData(C_EEG_Data& data, const QString& filename)
{
    C_AbstractParser::writeEEGData(data, filename);
}

void C_TextParser::writeTMData(C_TM& data, const QString& filename)
{
    C_AbstractParser::writeTMData(data, filename);
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
    dataOut << frame->eegType << delimeter;
    dataOut << frame->counter << delimeter;

    //Electrode Data
    for (i = 0; i < MAX_EEG_ELECTRODES; i++)
    {
        dataOut << frame->electrodeData[i];
    }

    //Contact Quality
    for (i = 0; i < MAX_EEG_ELECTRODES; i++)
    {
        dataOut << frame->contactQuality[i];
    }

    //The Rest of the Crap
    dataOut << frame->gyroX             << delimeter;
    dataOut << frame->gyroY             << delimeter;
    dataOut << frame->batteryPercentage << delimeter;
}

void C_TextParser::writeTMFrame(TM_Frame_t* frame)
{
    static bool firstFrame = true;

    if (firstFrame)
    {
        writeTMHeader();
        firstFrame = false;
    }

    dataOut << frame->timeStamp << delimeter;
    writeEEGFrame(&frame->eegFrame);
    writeBRSFrame(&frame->brsFrame);
    dataOut << frame->ledGroups[LED_FORWARD]->frequency  << delimeter;
    dataOut << frame->ledGroups[LED_BACKWARD]->frequency << delimeter;
    dataOut << frame->ledGroups[LED_RIGHT]->frequency    << delimeter;
    dataOut << frame->ledGroups[LED_LEFT]->frequency     << delimeter;
    dataOut << frame->eegConnectionStatus     << delimeter;
    dataOut << frame->pccConnectionStatus     << delimeter;
    dataOut << frame->brsConnectionStatus     << delimeter;
    dataOut << frame->flasherConnectionStatus << delimeter;

    dataOut << endl;
}

void C_TextParser::writeBRSFrame(BRS_Frame_t* frame)
{
    dataOut << frame->gpsData.altitude     << delimeter;
    dataOut << frame->gpsData.longitude    << delimeter;
    dataOut << frame->gpsData.altitude     << delimeter;
    dataOut << frame->gpsData.groundSpeed  << delimeter;
    dataOut << frame->usData.rangeToObject << delimeter;
    dataOut << frame->remoteCommand        << delimeter;
}

void C_TextParser::writeTMHeader()
{
    for (int i = 0; i < headerNames.tmFrameNames.size(); i++)
    {
        dataOut << headerNames.tmFrameNames.at(i) << delimeter;
    }
    dataOut << endl;
}

void C_TextParser::writeEEGHeader()
{
    for (int i = 0; i < headerNames.eegFrameNames.size(); i++)
    {
        dataOut << headerNames.eegFrameNames.at(i) << delimeter;
    }
    dataOut << endl;
}
