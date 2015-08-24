#include "pcc_cmd_control.h"


void initCmdControl()
{
	//Initialize servos
	configureServos();

	//Initialize LEDS OFF
	setLED(RED_LED, OFF);
	setLED(GREEN_LED, OFF);
}

void configureServos()
{
	unsigned int servo_stepval, servo_stepnow;
	unsigned int i;

	//Global Configuration
	TACCTL1	= OUTMOD_7;            // TACCR1 reset/set
	TACTL	= TASSEL_2 + MC_1;     // SMCLK, upmode
	TACCR0	= PWM_PERIOD-1;        // PWM Period
	TACCR1	= PWM_OFF;             // TACCR1 PWM Duty Cycle
	P1DIR	|= BIT6;               // P1.6 = output
	P1SEL	|= BIT6;               // P1.6 = TA1 output

	// Calculate the step value and define the current step, defaults to minimum.
	servo_stepval 	= ( (SERVO_MAX - SERVO_MIN) / SERVO_STEPS );
	servo_stepnow	= SERVO_MIN;

	// Fill up the LUT
	for (i = 0; i < SERVO_STEPS; i++) {
		servo_stepnow += servo_stepval;
		servo_lut[i] = servo_stepnow;
	}

	//Center the Servos
	CenterServos(PCC_CMD_NONE);
}

void sendServoCmd(unsigned char servo, ServoDirection direction)//direction
{
	//Status good
	setLED(RED_LED, OFF);
	setLED(GREEN_LED, ON);

	//Rotate servo in desired direction
	switch (direction)
	{
		case CLOCKWISE:
			rotateServo(servo, 180);//Move 90 degrees clockwise
		break;
		case COUNTER_CLOCKWISE:
			rotateServo(servo, 0);//Move 90 degrees counter-clockwise
		break;
	}
}

void rotateServo(unsigned char servo, ServoAngle angle)
{
	//Max Rotation is 180 degrees - 1
	if (angle >= 180)
	{
		angle = 180 - 1;
	}

	if (servo == SERVO_Y)
	{
		// Go to requested angle
		TA0CCR1 = servo_lut[angle];
	}
	else
	{
		//Don't know how to do multiple servos yet:(
	}

	__delay_cycles(MCU_CLOCK);
	TACCR1 = PWM_OFF;//stop Servo
}

//Container Function call for Forward Movement
void Forward(PCC_Command_Type lastCmd)
{
	println("Sending Logic Forward...");

	CenterServos(lastCmd);
	sendServoCmd(SERVO_X, CENTER);
	sendServoCmd(SERVO_Y, CLOCKWISE);
}

//Container Function call for Backward Movement
void Backward(PCC_Command_Type lastCmd)
{
	println("Sending Logic Backward...");

	CenterServos(lastCmd);
	sendServoCmd(SERVO_X, CENTER);
	sendServoCmd(SERVO_Y, COUNTER_CLOCKWISE);
}

//Container Function call for Right Movement
void Right(PCC_Command_Type lastCmd)
{
	println("Sending Logic Right...");

	CenterServos(lastCmd);
	sendServoCmd(SERVO_Y, CENTER);
	sendServoCmd(SERVO_X, CLOCKWISE);
}

//Container Function call for Left Movement
void Left(PCC_Command_Type lastCmd)
{
	println("Sending Logic Left...");

	CenterServos(lastCmd);
	sendServoCmd(SERVO_Y, CENTER);
	sendServoCmd(SERVO_X, COUNTER_CLOCKWISE);
}

//Container Function call for Stop Command
void Stop(PCC_Command_Type lastCmd)
{
	println("Sending Emergency Stop...");
	CenterServos(lastCmd);
}

//Reset the Servos in between sending commands
void CenterServos(PCC_Command_Type lastCmd)
{
	switch (lastCmd)
	{
		case PCC_FORWARD:
			sendServoCmd(SERVO_Y, COUNTER_CLOCKWISE);
		break;
		case PCC_BACKWARD:
			sendServoCmd(SERVO_Y, CLOCKWISE);
		break;
		case PCC_RIGHT:
			sendServoCmd(SERVO_X, COUNTER_CLOCKWISE);
		break;
		case PCC_LEFT:
			sendServoCmd(SERVO_X, CLOCKWISE);
		break;
		default:
			//Do Nothing
		break;
	}
}

//The Main Processing function of the PCC, Return the PCC Command that is being used
PCC_Command_Type processCommand(PCC_Command_Type currentCmd, PCC_Command_Type lastCmd)
{
	printNewLine();

	if (currentCmd == lastCmd)
	{
		return currentCmd;//We don't need to do anything here, keep the servos in the same position
	}

	switch (currentCmd)
	{
		case PCC_FORWARD:
			Forward(lastCmd);
		break;

		case PCC_RIGHT:
			Right(lastCmd);
		break;

		case PCC_LEFT:
			Left(lastCmd);
		break;

		case PCC_BACKWARD:
			Backward(lastCmd);
		break;

		case PCC_STOP:
			Stop(lastCmd);
		break;

		case PCC_TEST:
			blinkLEDs(5);//Blink LEDs for debugging
		break;
		default:
			println("Invalid Command");
			currentCmd = PCC_CMD_NONE;
		break;
	}

	return currentCmd;
}
