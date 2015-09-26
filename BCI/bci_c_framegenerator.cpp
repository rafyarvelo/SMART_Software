#include "bci_c_framegenerator.h"

BRS_Frame_t* FrameGenerator::GenerateBRSFrame()
{
    return GenerateBRSFrame(GeneratePCC_Command(), GetRandomFloat() / 155.36);
}

BRS_Frame_t* FrameGenerator::GenerateBRSFrame(PCC_Command_Type cmd, float rangeToObject)
{
    BRS_Frame_t* frame = BRS_Frame_t::create();

    frame->gpsData = GenerateGPSData();
    frame->usData.rangeToObject = rangeToObject;
    frame->remoteCommand = cmd;

    return frame;
}

EEG_Frame_t* FrameGenerator::GenerateEEGFrame()
{
    EEG_Frame_t* frame = EEG_Frame_t::create();

    int signs[] = { 1, -1 };

    frame->eegType = EEG_TYPE_EMOTIV; //We're not going to be ambitious
    frame->counter = GetRandomInt();

    //Electrode Data and Contact Quality
    for (int i = 0; i < MAX_EEG_ELECTRODES; i++)
    {
        frame->electrodeData[i]  =  GetRandomInt() * signs[GetRandomInt() % 2];
        frame->contactQuality[i] = (GetRandomInt() * 500) % 6000;
    }

    //The rest of the Crap
    frame->gyroX = GetRandomInt();
    frame->gyroY = GetRandomInt();
    frame->batteryPercentage = GetRandomInt();

    return frame;
}

TM_Frame_t*  FrameGenerator::GenerateTMFrame()
{
    return GenerateTMFrame(GeneratePCC_Command(), GetRandomFloat());
}

TM_Frame_t*  FrameGenerator::GenerateTMFrame(PCC_Command_Type cmd, float rangeToObject)
{
    TM_Frame_t* frame = TM_Frame_t::createFrame();

    frame->timeStamp = GetRandomInt();
    frame->eegFrame  = GenerateEEGFrame();
    frame->brsFrame  = GenerateBRSFrame(cmd, rangeToObject);
    //Leave LED Groups as the Default Groups

    frame->eegConnectionStatus     = CONNECTED;
    frame->pccConnectionStatus     = CONNECTED;
    frame->brsConnectionStatus     = CONNECTED;
    frame->flasherConnectionStatus = CONNECTED;

    return frame;
}

int FrameGenerator::GetRandomInt()
{
    static bool seeded = false;

    if (!seeded)
    {
        qsrand(QTime::currentTime().msec());
        seeded = true;
    }

    return (qrand() % RAND_MAX) + RAND_MIN;
}

float FrameGenerator::GetRandomFloat()
{
    return ((float) GetRandomInt() * 3.14159);
}

GPS_Data_t FrameGenerator::GenerateGPSData()
{
    GPS_Data_t gpsData;

    gpsData.latitude    = GetRandomFloat();
    gpsData.longitude   = GetRandomFloat();
    gpsData.altitude    = GetRandomFloat();
    gpsData.groundSpeed = GetRandomFloat();

    return gpsData;
}

PCC_Command_Type FrameGenerator::GeneratePCC_Command()
{
    //Sample Commands, With Highest Chance being PCC_CMD_NONE
    PCC_Command_Type commands[] =
    {
        PCC_FORWARD, PCC_CMD_NONE, PCC_BACKWARD, PCC_CMD_NONE,
        PCC_RIGHT  , PCC_CMD_NONE, PCC_LEFT    , PCC_CMD_NONE
    };

    return commands[GetRandomInt() % 8];
}
