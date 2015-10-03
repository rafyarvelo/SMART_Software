#ifndef BCI_C_TM
#define BCI_C_TM

#include <QList>
#include "../smart_config.h"
#include "../smart_debug_log.h"
#include "../smart_data_types.h"
#include "PCC/power_chair_command_constants.h"
#include "bci_c_eeg_data.h"
#include "bci_c_rvs.h"

class C_TM
{
public:
     C_TM();
    ~C_TM();
	
	static C_TM* Instance() { return new C_TM; }
	
    void addFrame(TM_Frame_t* frame, int index=-1);//back of list by default
    TM_Frame_t* GetFrame(int index=-1);//last frame by default
    TM_Frame_t* popFrame();
    sizeType size(){ return (sizeType) tmFrames.size();}
	
    //Clear the TM Data
    void clear();

    C_TM& operator =(C_TM& rhs);
private:
    QList<TM_Frame_t*> tmFrames;
};

#endif // BCI_C_TM

