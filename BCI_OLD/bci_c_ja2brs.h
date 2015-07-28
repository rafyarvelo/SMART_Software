#ifndef BCI_C_JA2BRS
#define BCI_C_JA2BRS

#include "../smart_config.h"
#include "../smart_debug_log.h"

class C_JA2BRS
{
public:		
	 C_JA2BRS(){}
	~C_JA2BRS(){}
	
	static C_JA2BRS* Instance()
	{
		return new C_JA2BRS;
	}
	
};

#endif // BCI_C_JA2BRS

