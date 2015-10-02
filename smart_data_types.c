#include "smart_data_types.h"
#include "../PCC/power_chair_command_constants.h"
#include <string.h>

//Default SMART Data Type Values

BRS_Frame_t* createBRSFrame()
{
	size_t dataSize = 0;
	BRS_Frame_t* ptr = NULL;

	//Create Frame
	dataSize = sizeof(BRS_Frame_t);
	ptr      = (BRS_Frame_t*) malloc(dataSize);
	memset(ptr, 0, dataSize);

	//Set Defaults
	ptr->MsgId = BRS2BCI_MSG_ID;
	ptr->remoteCommand = PCC_CMD_NONE;

	//Return new allocated frame
	return ptr;
}

BluetoothFrame_t* createBluetoothFrame()
{
	size_t dataSize = 0;
	BluetoothFrame_t* ptr = NULL;

	//Create Frame
	dataSize = sizeof(BluetoothFrame_t);
	ptr      = (BluetoothFrame_t*) malloc(dataSize);
	memset(ptr, 0, dataSize);

	//Default Values
	ptr->remoteCommand = PCC_CMD_NONE;

	//Return new allocated frame
	return ptr;
}

EEG_Frame_t* createEEGFrame()
{
	size_t dataSize = 0;
	EEG_Frame_t* ptr = NULL;

	//Create Frame
	dataSize = sizeof(EEG_Frame_t);
	ptr      = (EEG_Frame_t*) malloc(dataSize);
	memset(ptr, 0, dataSize);

	//Initialize Defaults
	ptr->eegType = DEFAULT_EEG_TYPE;

	//Return new allocated frame
	return ptr;
}

TM_Frame_t* createTMFrame()
{
	size_t dataSize = 0;
	TM_Frame_t* ptr = NULL;
	EEG_Frame_t* tmpEEGFrame = createEEGFrame();
	BRS_Frame_t* tmpBRSFrame = createBRSFrame();

	//Create Frame
	dataSize = sizeof(TM_Frame_t);
	ptr      = (TM_Frame_t*) malloc(dataSize);
	memset(ptr, 0, dataSize);

	//Initialize Defaults
	ptr->MsgId = BCI2BRS_MSG_ID;
	memcpy(&ptr->eegFrame,tmpEEGFrame, sizeof(EEG_Frame_t));
	memcpy(&ptr->brsFrame,tmpBRSFrame, sizeof(BRS_Frame_t));
	ptr->pccConnectionStatus     = NOT_CONNECTED;
	ptr->brsConnectionStatus     = NOT_CONNECTED;
	ptr->eegConnectionStatus     = NOT_CONNECTED;
	ptr->flasherConnectionStatus = NOT_CONNECTED;

	//Free Temp Memory Space
	free(tmpBRSFrame);
	free(tmpEEGFrame);

	//Return new allocated frame
	return ptr;
}

LED_Group_t* createLEDGroup(LED_Group_ID id)
{
	size_t dataSize = 0;
	LED_Group_t* ptr = NULL;

	//Create LED Group
	dataSize = sizeof(LED_Group_t);
	ptr      = (LED_Group_t*) malloc(dataSize);
	memset(ptr, 0, dataSize);

	//Initialize Defaults
	ptr->id = id;
	switch (id)
	{
		case LED_FORWARD:
			ptr->frequency = LED_FORWARD_FREQ_DEFAULT;
		break;
		case LED_BACKWARD:
			ptr->frequency = LED_BACKWARD_FREQ_DEFAULT;
		break;
		case LED_RIGHT:
			ptr->frequency = LED_RIGHT_FREQ_DEFAULT;
		break;
		case LED_LEFT:
			ptr->frequency = LED_LEFT_FREQ_DEFAULT;
		break;
	}

	return ptr;
}
