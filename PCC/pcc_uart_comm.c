
#include <string.h>
#include "pcc_uart_comm.h"

void printStr(const char *str)
{
	unsigned int i, len = strlen(str);

	for (i = 0; i < len; i++)
		OUTCHAR_UART(str[i]);
}

void println(const char* str)
{
	printStr(str);
	printNewLine();
}

void printNewLine(void)
{
	OUTCHAR_UART('\r');
	OUTCHAR_UART('\n');
}

void OUTCHAR_UART(unsigned char A){
//---------------------------------------------------------------
//***************************************************************
//---------------------------------------------------------------
// IFG2 register (1) = 1 transmit buffer is empty,
// UCA0TXBUF 8 bit transmit buffer
// wait for the transmit buffer to be empty before sending the
// data out
do{
  }while ((IFG2&0x02)==0);

// send the data to the transmit buffer
UCA0TXBUF =A;
}


unsigned char INCHAR_UART(void){
//---------------------------------------------------------------
//***************************************************************
//---------------------------------------------------------------
// IFG2 register (0) = 1 receive buffer is full,
// UCA0RXBUF 8 bit receive buffer
// wait for the receive buffer is full before getting the data
do{
}while ((IFG2&0x01)==0);
// go get the char from the receive buffer
return (UCA0RXBUF);
}


void  Init_UART(void){
//---------------------------------------------------------------
// Set up the MSP430g2553 for a 1 MHZ clock speed
// For the version 1.5 of the launchpad MSP430g2553
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
BCSCTL1=CALBC1_1MHZ;
DCOCTL=CALDCO_1MHZ;
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//---------------------------------------------------------------

//---------------------------------------------------------------
// Initialization code to set up the uart on the experimenter
// board to 8 data,
// 1 stop, no parity, and 9600 baud, polling operation
//---------------------------------------------------------------
//---------------------------------------------------------------
// Set up the MSP430g2553 for 1.2 for the transmit pin and 1.1 receive pin
// For the version 1.5 of the launchpad MSP430g2553
// Need to connect the UART to port 1.
// P1SEL, P1sel2 = off, 01 = primary I/O, 10 = Reserved, 11 = secondary I/O for UART
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
P1SEL =  0x06;    // transmit and receive to port 1 bits 1 and 2
P1SEL2 = 0x06;   // transmit and receive to port 1 bits 1 and 2
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//---------------------------------------------------------------
		 // Bits p1.2 transmit and p1.1 receive
UCA0CTL0=0;      // 8 data, no parity 1 stop, uart, async
		 // (7)=1 (parity), (6)=1 Even, (5)= 0 lsb first,
 		 // (4)= 0 8 data / 1 7 data,
		 // (3) 0 1 stop 1 / 2 stop, (2-1)  -- UART mode,
                 // (0) 0 = async
//---------------------------------------------------------------
// Set up the MSP430g2553 for using the master clock MLK - SMCLK
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
UCA0CTL1= 0x81;
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		 // select MLK
		 // software reset the UART
		 // (7-6) 00 UCLK, 01 ACLK (32768 hz), 10 SMCLK,
 		 // 11 SMCLK
		 // (0) = 1 reset
//---------------------------------------------------------------
UCA0BR1=0;	 // upper byte of divider clock word
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
UCA0BR0=0x68;    // clock divide from a clock to bit clock
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
 		 //  1,000,000/9600 =104.16 , 104 = 0x68
                 // UCA0BR1:UCA0BR0 two 8 bit reg to from 16 bit
                 // clock divider
		 // for the baud rate
UCA0MCTL=0x06;
                 // low frequency mode module 6 modulation pater
                 // used for the bit clock
UCA0STAT=0;      // do not loop the transmitter back to the
                 // receiver for echoing
		 // (7) = 1 echo back trans to rec
		 // (6) = 1 framing, (5) = 1 overrun, (4) =1 Parity,
		 // (3) = 1 break
		 // (0) = 2 transmitting or receiving data
//---------------------------------------------------------------
// Set up the MSP430g2553 for using the master clock MLK - SMCLK
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
UCA0CTL1=0x80;
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
      	 // take UART out of reset
//---------------------------------------------------------------

IE2=0; 	 // turn transmit interrupts off

//---------------------------------------------------------------
//***************************************************************
//---------------------------------------------------------------
		// IFG2 register (0) = 1 receiver buffer is full,
		// UCA0RXIFG
		// IFG2 register (1) = 1 transmit buffer is empty,
		// UCA0RXIFG
		// UCA0RXBUF 8 bit receiver buffer
		// UCA0TXBUF 8 bit transmit buffer
}



//Display an integer to HyperTerminal
void Hyperterminal_display(int i)
{
//converts to ascii because hyperterminal can only input and output ascii

// if the bits are a number 0 9
	if (i >= 0x00 && i <= 0x09)
		// +30 to obtain the hex ASCII code for the number
		i += 0x30;
	// if the bits are a letter A F
	else
		// +37 to obtain the hex ASCII code for the letter
		i += 0x37;
	OUTCHAR_UART(i);
}

char toggleCase(char c)
{
	//if uppercase letter
	if (c >= 65 && c <= 90)
		c += 32;//make lowercase
	//if lowercase
	else if (c >= 97 && c <= 122)
		c -= 32;//make uppercase
	else
		; //invalid input, do nothing

	return c;
}

//convert hexadecimal character to ASCII
char hex2ASCII(char c)
{
	//if hex value is a number 0-9
	if (c >= 0 && c <= 9)
		c += 48;
	//if hex value is a letter A-F
	else if ( c >= 10 && c <= 15 )
		c += 55;
	else
		;//input not valid, do nothing

	return c;
}


//convert from ASCII value to HEX value
char ASCII2Hex(char c)
{
	//if c is 0-9
	if(c >= 48 && c <= 57)
			c -= 48;
	//if c is A-F
	else if(c >= 65 && c <= 70)
		c -= 55;

	//if c is a-f
	else if (c >= 97 && c <= 102)
		c -= 87;
	else
		; //input is not valid, do the same

	return c;
}

//Return the Contents of the Receive buffer
unsigned char currentChar(void)
{
	return (UCA0RXBUF);
}

void printInteger(unsigned long num)
{
	char buffer[10];
	unsigned long long SIZE;
	unsigned short i = 0;

	for (SIZE = 10 ^ 9; SIZE > 0; SIZE /= 10)
	{
		if (num >= SIZE)
		{
			buffer[i++] = hex2ASCII( ((num % (SIZE * 10)) / 10) );
			num /= 10;
		}
	}
	buffer[i] = '\0';

	printStr(buffer);//output to terminal
}
