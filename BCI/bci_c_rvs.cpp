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

void C_RVS::emitRVS(LED_Group_ID id)
{
	
}

void C_RVS::SetFrequency(LED_Group_ID id, float frequency)
{
	
}


	
