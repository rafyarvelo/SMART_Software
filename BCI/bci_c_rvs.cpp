#include "bci_c_rvs.h"

C_RVS::C_RVS()
{
    ledGroups[LED_FORWARD]  = createLEDGroup(LED_FORWARD);
    ledGroups[LED_BACKWARD] = createLEDGroup(LED_BACKWARD);
    ledGroups[LED_RIGHT]    = createLEDGroup(LED_RIGHT);
    ledGroups[LED_LEFT]     = createLEDGroup(LED_LEFT);
}

C_RVS::~C_RVS()
{
    for (int id = LED_FORWARD; id < NUM_LED_GROUPS; id++)
    {
        delete ledGroups[id];
    }
}

LED_Group_t*  C_RVS::GetLEDGroup(LED_Group_ID id)
{
	if (id < 0 || id > NUM_LED_GROUPS)
	{
		id = LED_FORWARD;//return forward group by default
	}
    return ledGroups[id];
}

void C_RVS::SetFrequency(LED_Group_ID id, unsigned short frequency)
{
	if (id < 0 || id > NUM_LED_GROUPS)
	{
		;//invalid id, do nothing
	}	
	else
	{
        ledGroups[id]->frequency = frequency;
	}
}

void C_RVS::Generate()
{
    //Do Nothing for now..
}


	
