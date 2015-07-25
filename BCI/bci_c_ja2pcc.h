#ifndef BCI_C_JA2PCC
#define BCI_C_JA2PCC

#include "../PCC/power_chair_command_constants.h"
#include "../smart_config.h"
#include "../smart_debug_log.h"

class C_JA2PCC
{
public:
	 C_JA2PCC(){}
	~C_JA2PCC(){}
	
	static C_JA2PCC* Instance() { return new C_JA2PCC;}
	
	void SetCommand(PCC_Command_Type cmd);
	void SendCommand();
	
	void Connect();
	bool ConnectionStatus();
	
private:
	PCC_Command_Type currentCommand;
	bool             connectionStatus;
};

#endif // BCI_C_JA2PCC

