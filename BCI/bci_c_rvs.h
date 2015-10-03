
#ifndef BCI_C_RVS
#define BCI_C_RVS

#include "smart_data_types.h"
#include "smart_debug_log.h"


class C_RVS
{
public:
      C_RVS();
     ~C_RVS();

	static C_RVS* Instance(){ return new C_RVS; }
    LED_Group_t*  GetLEDGroup(LED_Group_ID id);
    void SetFrequency(LED_Group_ID id, unsigned short frequency);

    void Generate(); //Creates RVS frequencies

protected:
	SMART_DEBUG_LOG* debugLog;
    LED_Group_t*     ledGroups[NUM_LED_GROUPS];
};

#endif // BCI_C_RVS

