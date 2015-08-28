#include "pcc_cmd_control.h"

//Initialize PCC Commands
void initCmdControl()
{
	//Initialize servos
	configureServos();

	//Initialize LEDS OFF
	setLED(RED_LED, OFF);
	setLED(GREEN_LED, OFF);
}

//This Sets up the Timers and PWM for the Servos
void configureServos()
{
	unsigned int servo_stepval, servo_stepnow;
	unsigned int i;

	//Global Configuration
	TACCTL1 = PWM_RESET_SET;   // TACCR1 reset/set
	TACTL   = TIMERA_SMCLK_UP; // SMCLK, upmode
	TACCR0  = PWM_PERIOD-1;    // PWM Period
	TACCR1  = PWM_OFF;         // TACCR1 PWM Duty Cycle
	P1DIR  |= BIT6;            // P1.6 = output
	P1SEL  |= BIT6;            // P1.6 = TA1 output

	// Calculate the step value and define the current step, defaults to minimum.
	servo_stepval = ( (SERVO_MAX - SERVO_MIN) / SERVO_STEPS );
	servo_stepnow = SERVO_MIN;

	// Fill up the Look-Up-Table
	for (i = 0; i < SERVO_STEPS; i++)
	{
		servo_stepnow += servo_stepval;
		servo_lut[i] = servo_stepnow;
	}

	//Center the Servos
	CenterServos(PCC_CMD_NONE);
}


//Limit the Direction of the Serov to Forward, Back, and Center
void sendServoCmd(unsigned char servo, ServoDirection direction)//direction
{
	//Rotate servo in desired direction
	switch (direction)
	{
		case SERVO_FORWARD:
			rotateServo(servo, 179);//Move to +90 degrees
		break;
		case SERVO_BACKWARD:
			rotateServo(servo, 0);  //Move to -90 degrees
			break;
		case SERVO_CENTER:
			rotateServo(servo, 90); //Return to Center
		break;
	}
}

//Perform Actual Rotation of Servos
void rotateServo(unsigned char servo, ServoAngle angle)
{
	//Protect from invalid Rotation Commands
	angle = angle % MAX_ROTATION;

	if (servo == SERVO_Y)
	{
		// Go to requested angle
		TA0CCR1 = servo_lut[angle];
	}
	else
	{
		//Don't know how to do multiple servos yet:(
	}

	//Short Delay, Then turn PWM OFF
	__delay_cycles(MCU_CLOCK);
	TACCR1 = PWM_OFF;
}

//Container Function call for Forward Movement
void Forward()
{
	println("Sending Logic Forward...");

	sendServoCmd(SERVO_X, SERVO_CENTER);
	sendServoCmd(SERVO_Y, SERVO_FORWARD);
}

//Container Function call for Backward Movement
void Backward()
{
	println("Sending Logic Backward...");

	sendServoCmd(SERVO_X, SERVO_CENTER);
	sendServoCmd(SERVO_Y, SERVO_BACKWARD);
}

//Container Function call for Right Movement
void Right()
{
	println("Sending Logic Right...");

	sendServoCmd(SERVO_Y, SERVO_CENTER);
	sendServoCmd(SERVO_X, SERVO_FORWARD);
}

//Container Function call for Left Movement
void Left()
{
	println("Sending Logic Left...");

	sendServoCmd(SERVO_Y, SERVO_CENTER);
	sendServoCmd(SERVO_X, SERVO_BACKWARD);
}

//Container Function call for Stop Command
void Stop()
{
	println("Sending Emergency Stop...");
	CenterServos();
}

//Set the Servo Positions to Centerered
void CenterServos()
{
	sendServoCmd(SERVO_X, SERVO_CENTER);
	sendServoCmd(SERVO_Y, SERVO_CENTER);
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
			Forward();
		break;

		case PCC_RIGHT:
			Right();
		break;

		case PCC_LEFT:
			Left();
		break;

		case PCC_BACKWARD:
			Backward();
		break;

		case PCC_STOP:
			Stop();
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
