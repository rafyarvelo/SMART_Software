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

EEG_Frame_t* C_TextParser::readEEGFrame()
{
    EEG_Frame_t* buffer = createEEGFrame();

    return buffer;
}

TM_Frame_t*  C_TextParser::readTMFrame()
{
    TM_Frame_t* buffer = createTMFrame();

    return buffer;
}

void C_TextParser::writeEEGFrame(EEG_Frame_t* frame)
{
    int i = 0;
    static bool firstFrame = true;

    if (firstFrame)
    {
        writeEEGHeader();
        firstFrame = false;
    }

    stream << FILE_FORMAT::EEG_TYPES[0] << delimeter;
    stream << frame->counter << delimeter;

    //Electrode Data
    for (i = 0; i < MAX_EEG_ELECTRODES; i++)
    {
        stream << (float) frame->electrodeData[i] / 100.0 << delimeter;
    }

    //Contact Quality
    for (i = 0; i < MAX_EEG_ELECTRODES; i++)
    {
        stream << frame->contactQuality[i] << delimeter;
    }

    //The Rest of the Crap
    stream << frame->gyroX             << delimeter;
    stream << frame->gyroY             << delimeter;
    stream << frame->batteryPercentage << delimeter;
    stream << endl;
}

void C_TextParser::writeTMFrame(TM_Frame_t* frame)
{
    static bool firstFrame = true;

    if (firstFrame)
    {
        writeTMHeader();
        firstFrame = false;
    }

    //Write all the Data to the text File
    stream << (float) frame->timeStamp / 1000.0         << delimeter;
    stream << FILE_FORMAT::BCI_STATES[frame->bciState]  << delimeter;
    stream << static_cast<char>(frame->lastCommand)     << delimeter;
    stream << FILE_FORMAT::CONFIDENCES[frame->lastConfidence]              << delimeter;
    stream << static_cast<char>(frame->processingResult.command)           << delimeter;
    stream << FILE_FORMAT::CONFIDENCES[frame->processingResult.confidence] << delimeter;

    writeBRSFrame(&frame->brsFrame);

    stream << frame->ledForward .frequency   << delimeter;
    stream << frame->ledBackward.frequency   << delimeter;
    stream << frame->ledRight   .frequency   << delimeter;
    stream << frame->ledLeft    .frequency   << delimeter;
    stream << FILE_FORMAT::CONN_STATUSES[frame->eegConnectionStatus]     << delimeter;
    stream << FILE_FORMAT::CONN_STATUSES[frame->pccConnectionStatus]     << delimeter;
    stream << FILE_FORMAT::CONN_STATUSES[frame->brsConnectionStatus]     << delimeter;
    stream << FILE_FORMAT::CONN_STATUSES[frame->flasherConnectionStatus] << delimeter;

    stream << endl;
}

void C_TextParser::writeBRSFrame(BRS_Frame_t* frame)
{
    stream << frame->sensorData.gpsData.altitude     << delimeter;
    stream << frame->sensorData.gpsData.longitude    << delimeter;
    stream << frame->sensorData.gpsData.altitude     << delimeter;
    stream << frame->sensorData.gpsData.groundSpeed  << delimeter;
    stream << frame->sensorData.rangeFinderData.rangeFront << delimeter;
    stream << frame->sensorData.rangeFinderData.rangeBack  << delimeter;
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
