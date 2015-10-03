#include "bci_c_framegenerator.h"

BRS_Frame_t* FrameGenerator::GenerateBRSFrame(BRS_Frame_t* pFrame)
{
    return GenerateBRSFrame(pFrame, GeneratePCC_Command());
}

BRS_Frame_t* FrameGenerator::GenerateBRSFrame(BRS_Frame_t* pFrame, PCC_Command_Type cmd)
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

    int signs[] = { 1, -1 };

    pFrame->eegType = EEG_TYPE_EMOTIV; //We're not going to be ambitious
    pFrame->counter = GetRandomInt();

    //Electrode Data and Contact Quality
    for (int i = 0; i < MAX_EEG_ELECTRODES; i++)
    {
        pFrame->electrodeData[i]  =  GetRandomInt() * signs[GetRandomInt() % 2];
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
    return GenerateTMFrame(pFrame, GeneratePCC_Command());
}

TM_Frame_t*  FrameGenerator::GenerateTMFrame(TM_Frame_t* pFrame, PCC_Command_Type cmd)
{
    if (!pFrame)
    {
        pFrame = createTMFrame();
    }

    //Update The TM Frame
    pFrame->timeStamp = GetRandomInt();
    pFrame->bciState  = BCI_STANDBY;
    GenerateEEGFrame(&pFrame->eegFrame);
    GenerateBRSFrame(&pFrame->brsFrame, cmd);

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

    return (qrand() % RAND_MAX) + RAND_MIN;
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
        memset(reinterpret_cast<void*>(pData),0,sizeof(SensorData_t));
    }

    pData->gpsData.latitude    = GetRandomFloat();
    pData->gpsData.longitude   = GetRandomFloat();
    pData->gpsData.altitude    = GetRandomFloat();
    pData->gpsData.groundSpeed = GetRandomFloat();

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
