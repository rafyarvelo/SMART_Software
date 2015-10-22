#include "bci_c_framegenerator.h"

BRS_Frame_t* FrameGenerator::GenerateBRSFrame(BRS_Frame_t* pFrame)
{
    return GenerateBRSFrame(GeneratePCC_Command(), pFrame);
}

BRS_Frame_t* FrameGenerator::GenerateBRSFrame(PCC_Command_Type cmd, BRS_Frame_t* pFrame)
{
    if (!pFrame)
    {
        pFrame = createBRSFrame();
    }

    GenerateSensorData(&pFrame->sensorData);
    pFrame->remoteCommand = cmd;

    return pFrame;
}

EEG_Frame_t* FrameGenerator::GenerateEEGFrame(EEG_Frame_t* pFrame)
{
    if (!pFrame)
    {
        pFrame = createEEGFrame();
    }

    pFrame->eegType = EEG_TYPE_EMOTIV; //We're not going to be ambitious
    pFrame->counter = GetRandomInt();

    //Electrode Data and Contact Quality
    for (int i = 0; i < MAX_EEG_ELECTRODES; i++)
    {
        pFrame->electrodeData[i]  = (GetRandomInt());
        pFrame->contactQuality[i] = (GetRandomInt() * 500) % 6000;
    }

    //The rest of the Crap
    pFrame->gyroX = GetRandomInt();
    pFrame->gyroY = GetRandomInt();
    pFrame->batteryPercentage = GetRandomInt();

    return pFrame;
}

TM_Frame_t*  FrameGenerator::GenerateTMFrame(TM_Frame_t* pFrame)
{
    return GenerateTMFrame(GeneratePCC_Command(), pFrame);
}

TM_Frame_t*  FrameGenerator::GenerateTMFrame(PCC_Command_Type cmd, TM_Frame_t* pFrame)
{
    if (!pFrame)
    {
        pFrame = createTMFrame();
    }

    //Update The TM Frame
    pFrame->timeStamp      = GetRandomInt();
    pFrame->bciState       = BCI_STANDBY;
    pFrame->lastCommand    = GeneratePCC_Command();
    pFrame->lastConfidence = UNSURE;
    pFrame->processingResult.command    = GeneratePCC_Command();
    pFrame->processingResult.confidence = UNSURE;
    GenerateBRSFrame(cmd,&pFrame->brsFrame);

    /*Leave LED Groups as the Default Groups*/

    pFrame->eegConnectionStatus     = CONNECTED;
    pFrame->pccConnectionStatus     = CONNECTED;
    pFrame->brsConnectionStatus     = CONNECTED;
    pFrame->flasherConnectionStatus = CONNECTED;

    return pFrame;
}

int FrameGenerator::GetRandomInt()
{
    static bool seeded = false;

    if (!seeded)
    {
        qsrand(QTime::currentTime().msec());
        seeded = true;
    }

    return ((qrand() % RANDOM_MAX) + RANDOM_MIN);
}

float FrameGenerator::GetRandomFloat()
{
    return ((float) GetRandomInt() * 3.14159);
}

SensorData_t* FrameGenerator::GenerateSensorData(SensorData_t* pData)
{
    if (!pData)
    {
        pData = new SensorData_t;
    }

    pData->gpsData.latitude    = GetRandomFloat();
    pData->gpsData.longitude   = GetRandomFloat();
    pData->gpsData.altitude    = GetRandomFloat() / 200;
    pData->gpsData.groundSpeed = GetRandomFloat() / 100;

    pData->rangeFinderData.rangeBack  = GetRandomFloat() / 100;
    pData->rangeFinderData.rangeFront = GetRandomFloat() / 100;

    return pData;
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
