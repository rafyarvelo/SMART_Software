#include "bci_c_rvs.h"

C_RVS::C_RVS()
{
	ledGroups = LED_Group::DefaultGroups();
}

C_RVS::~C_RVS()
{
	if (ledGroups)
	{
		for (int i = 0; i < NUM_LED_GROUPS; i++)
		{
			delete ledGroups[i];
		}
		delete[] ledGroups;
	}
}

LED_Group*  C_RVS::GetLEDGroup(LED_Group_ID id)
{
	if (id < 0 || id > NUM_LED_GROUPS)
	{
		id = LED_FORWARD;//return forward group by default
	}
	return ledGroups[id];
}

LED_Group** C_RVS::GetAllLEDGroups()
{
    return ledGroups;
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


	
