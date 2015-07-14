#include "pcc_cmd_control.h"

//This Function Pointer references the last command that was successfully
//sent to the Power chair.
void  (*pLastProcessedCmd)() = PCC_CMD_NONE;

//The number of Commands that have arrived that are not valid
volatile unsigned short invalidCmdCount = 0;

//The Maximum amount of commands missed before stopping power chair commands
volatile const unsigned short maxInvalidCmdCount = 5;

//initilization check
volatile unsigned short pcc_cntrl_initialized = FALSE;

void initCmdControl()
{
	pLastProcessedCmd = PCC_CMD_NONE;
	invalidCmdCount   = 0;

	//Initialize servos
	sendGPIO(PORT1_REF, SERVO_X, LOGIC_LOW);
	sendGPIO(PORT1_REF, SERVO_Y, LOGIC_LOW);

	pcc_cntrl_initialized = TRUE;
}

void sendServoCmd(unsigned char servo, ServoDirection direction)//direction is
{
	//Status good
	setLED(RED_LED, OFF);
	setLED(GREEN_LED, ON);

	sendGPIO(PORT1_REF, servo, LOGIC_HIGH);
	sleep(PULSE_DURATION);
	sendGPIO(PORT1_REF, servo, LOGIC_LOW);
}

void Forward()
{
	println("Sending Logic Forward...");

	sendServoCmd(SERVO_Y, CLOCKWISE);
	pLastProcessedCmd = Forward;
}

void Backward()
{
	println("Sending Logic Backward...");

	sendServoCmd(SERVO_Y, COUNTER_CLOCKWISE);
	pLastProcessedCmd = Backward;
}

void Right()
{
	println("Sending Logic Right...");

	sendServoCmd(SERVO_Y, CLOCKWISE);
	pLastProcessedCmd = Right;
}

void Left()
{
	println("Sending Logic Left...");

	sendServoCmd(SERVO_X, CLOCKWISE);
	pLastProcessedCmd = Left;
}

void Stop()
{
	println("Sending Emergency Stop...");

	sendServoCmd(SERVO_X, COUNTER_CLOCKWISE);
	pLastProcessedCmd = Stop;
}

void printInvalidCmdCount(void)
{
	char invalidCmdStr[50];
	sprintf(invalidCmdStr,"Invalid Command, Count = %d", invalidCmdCount);
	printStr(invalidCmdStr);
}

void SendLastCommand()
{
	if (pLastProcessedCmd != PCC_CMD_NONE)
		pLastProcessedCmd();
}

void sleep(unsigned short time)
{
	volatile unsigned int i;
	volatile unsigned const int SCALE_FACTOR = 100;

	for (i = 0; i < time * SCALE_FACTOR; i++)
		;//sleep
}

//The Main Processing function of the PCC
void processCommand(unsigned char cmd)
{
	//initialize if necessary
	if (!pcc_cntrl_initialized)
		initCmdControl();

	unsigned long i = 0;
	for (i = 0; i < 500; i++)
	{
		printInteger(i);
	}

	printNewLine();
	switch (cmd)
	{
		case PCC_FORWARD:
			Forward();
			invalidCmdCount = 0;
		break;

		case PCC_RIGHT:
			Right();
			invalidCmdCount = 0;
		break;

		case PCC_LEFT:
			Left();
			invalidCmdCount = 0;
		break;

		case PCC_BACKWARD:
			Backward();
			invalidCmdCount = 0;
		break;

		case PCC_STOP:
			Stop();
			invalidCmdCount = 0;
		break;

		default:
			if (++invalidCmdCount >= maxInvalidCmdCount)
			{
				println("Maximum amount of Invalid Commands Processed, Stopping PCC Commands...");
				setLED(RED_LED, ON);
				setLED(GREEN_LED, OFF);
				pLastProcessedCmd = NULL;
			}
			else//Keep sending the last successfully processed command until the retry count is reached
			{
				SendLastCommand();
				printInvalidCmdCount();
			}

			break;
	}
}
