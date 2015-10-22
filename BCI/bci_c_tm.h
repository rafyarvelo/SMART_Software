#ifndef BCI_C_TM
#define BCI_C_TM

#include <QList>
#include "../smart_config.h"
#include "../smart_debug_log.h"
#include "../smart_data_types.h"
#include "PCC/power_chair_command_constants.h"
#include "bci_c_rvs.h"
#include "bci_c_safequeue.h"

#define TM_BUFFER_SIZE 30

//A Simple Wrapper around the Safe Queue Class to hold the TM Frames
class C_TM
{
public:
     C_TM(unsigned int bufferSize = TM_BUFFER_SIZE);
    ~C_TM();

    void addFrame(TM_Frame_t* frame);
    TM_Frame_t* GetFrame();
    sizeType size(){ return (sizeType) tmFrames->itemsAvailable();}

    C_TM& operator =(C_TM& rhs);

private:
    C_SafeQueue<TM_Frame_t*>* tmFrames;
};

#endif // BCI_C_TM

