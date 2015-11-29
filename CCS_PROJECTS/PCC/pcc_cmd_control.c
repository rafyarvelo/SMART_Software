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

	//Timer A1 Configuration
	TA1CTL   = TIMERA_SMCLK_UP; // SMCLK, upmode
	TA1CCR0  = PWM_PERIOD-1;    // PWM Period
	BCSCTL1  = CALBC1_1MHZ;     // Set DCO range
	DCOCTL   = CALDCO_1MHZ;     // Set DCO step and modulation

	//Servo Y Configuration
	TA1CCTL1 =  PWM_RESET_SET;   // TA1CCR1 reset/set
	TA1CCR1  =  PWM_OFF;         // TA1CCR1 PWM Duty Cycle
	P2DIR   |=  SERVO_Y;         // P2.1 = output
	P2SEL   |=  SERVO_Y;         // P2.1 = TA1.1 output
	P2SEL2  &= ~SERVO_Y;         // Clear Timer Bit (Just to be safe)

	//Servo X Configuration
	TA1CCTL2 =  PWM_RESET_SET;   // TA1CCR1 reset/set
	TA1CCR2  =  PWM_OFF;         // TA1CCR1 PWM Duty Cycle
	P2DIR   |=  SERVO_X;         // P2.4 = output
	P2SEL   |=  SERVO_X;         // P2.4 = TA1.1 output
	P2SEL2  &= ~SERVO_X;         // Clear Timer Bit (Just to be safe)

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
			rotateServo(servo, ANGLE_FORWARD);
		break;
		case SERVO_BACKWARD:
			rotateServo(servo, ANGLE_BACKWARD);
			break;
		case SERVO_CENTER:
			rotateServo(servo, ANGLE_CENTER);
		break;
	}
}

//Perform Actual Rotation of Servos
void rotateServo(unsigned char servo, ServoAngle angle)
{
	//Protect from invalid Rotation Commands
	angle = angle % MAX_ROTATION;

	//Change the Duty Cycle of the Correponding PWM Pin
	if (servo == SERVO_Y)
	{
		// Go to requested angle
		TA1CCR1 = servo_lut[angle];
	}
	else
	{
		TA1CCR2 = servo_lut[angle];
	}
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
	sendServoCmd(SERVO_X, SERVO_BACKWARD);
}

//Container Function call for Left Movement
void Left()
{
	println("Sending Logic Left...");

	sendServoCmd(SERVO_Y, SERVO_CENTER);
	sendServoCmd(SERVO_X, SERVO_FORWARD);
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
	println("Centering Servos...");
	sendServoCmd(SERVO_X, SERVO_CENTER);
	sendServoCmd(SERVO_Y, SERVO_CENTER);
}

//Disable PWM to Servos
void DisablePWM()
{
	println("Disabling PWM Signal...");
	TA1CCR1 = PWM_OFF;
	TA1CCR2 = PWM_OFF;
}

//Test all Possible Servo Commands
void ServoTest()
{
	println("Running Initial Servo Tests...");
	processCommand(PCC_TEST    , PCC_CMD_NONE);
	__delay_cycles(MCU_CLOCK);

	processCommand(PCC_FORWARD , PCC_CMD_NONE);
	__delay_cycles(MCU_CLOCK);

	processCommand(PCC_BACKWARD, PCC_CMD_NONE);
	__delay_cycles(MCU_CLOCK);

	processCommand(PCC_RIGHT   , PCC_CMD_NONE);
	__delay_cycles(MCU_CLOCK);

	processCommand(PCC_LEFT    , PCC_CMD_NONE);
	__delay_cycles(MCU_CLOCK);

	//Return the Servos to the Center
	CenterServos();
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

		case PCC_TEST: //Debug the UART by blinking the LEDs
			println("UART TEST!");
			//blinkLEDs(5);
		break;

		case PCC_OFF:
			DisablePWM();
		break;

		default:
			println("Invalid Command");
			currentCmd = PCC_CMD_NONE;
		break;
	}

	return currentCmd;
}
