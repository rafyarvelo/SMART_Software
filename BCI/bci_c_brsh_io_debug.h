#ifndef BCI_C_BRSH_IO_DEBUG_H
#define BCI_C_BRSH_IO_DEBUG_H

#include "bci_c_brsh_io.h"

class C_BRSH_IO_Debug : public C_BRSH_IO, public C_Singleton<C_BRSH_IO_Debug>
{
    Q_OBJECT
public:
    C_BRSH_IO_Debug();
    virtual ~C_BRSH_IO_Debug(){}

    //Send TM to the BRSH
    virtual void SendTMFrame(TM_Frame_t* pFrame){}

    //Return the latest BRS Data
    virtual BRS_Frame_t* GetLatestBRSFrame(){ return BRS_Frame_t::create();}

    virtual ConnectionStatusType connect() { return CONNECTED;}
};

#endif // BCI_C_BRSH_IO_DEBUG_H
