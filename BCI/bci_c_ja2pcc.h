#ifndef BCI_C_JA2PCC
#define BCI_C_JA2PCC

#include "../PCC/power_chair_command_constants.h"

class C_JA2PCC
{
public:
	 C_JA2PCC(){}
	~C_JA2PCC(){}
	
	static C_JA2PCC* Instance();
	
	void SetCommand(PCC_Command_Type cmd);
	void SendCommand();
	
	void connect();
	bool connectionStatus();
	
private:
	PCC_Command_Type currentCommand;
	bool             connectionStatus;
};

#endif // BCI_C_JA2PCC

