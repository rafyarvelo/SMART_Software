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
    static BRS_Frame_t* GenerateBRSFrame();
    static BRS_Frame_t* GenerateBRSFrame(PCC_Command_Type cmd, float rangeToObject);
    static EEG_Frame_t* GenerateEEGFrame();
    static TM_Frame_t*  GenerateTMFrame();
    static TM_Frame_t*  GenerateTMFrame(PCC_Command_Type cmd, float rangeToObject);

    static GPS_Data_t       GenerateGPSData();
    static PCC_Command_Type GeneratePCC_Command();

private:
    static int   GetRandomInt();
    static float GetRandomFloat();
};

#endif // BCI_C_FRAMEGENERATOR_H
