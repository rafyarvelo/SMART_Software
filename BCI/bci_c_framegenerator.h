#ifndef BCI_C_FRAMEGENERATOR_H
#define BCI_C_FRAMEGENERATOR_H

#include "bci_c_tm.h"
#include <QTime>

#define RAND_MIN 1
#define RAND_MAX 128

//Use this class to Generate TM Frames for Debugging/Test Purposes
class FrameGenerator
{
public:

    //Generate Each Type of Frame, Simply Pass in the Address where the Random Data should be populated.
    //Alternatively, Pass 0 or no arguments to allocate memory and return the address of the new random data
    static BRS_Frame_t* GenerateBRSFrame(BRS_Frame_t* pFrame=0);
    static BRS_Frame_t* GenerateBRSFrame(BRS_Frame_t* pFrame=0, PCC_Command_Type cmd=PCC_CMD_NONE);
    static EEG_Frame_t* GenerateEEGFrame(EEG_Frame_t* pFrame=0);
    static TM_Frame_t*  GenerateTMFrame(TM_Frame_t*   pFrame=0);
    static TM_Frame_t*  GenerateTMFrame(TM_Frame_t*   pFrame=0, PCC_Command_Type cmd=PCC_CMD_NONE);

    static SensorData_t*    GenerateSensorData(SensorData_t* pData=0);
    static PCC_Command_Type GeneratePCC_Command();

private:
    static int   GetRandomInt();
    static float GetRandomFloat();
};

#endif // BCI_C_FRAMEGENERATOR_H
