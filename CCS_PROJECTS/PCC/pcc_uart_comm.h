/*
 * pcc_uart_comm.h
 *
 *  Created on: Jul 5, 2015
 *      Author: Rafy
 */

#ifndef PCC_UART_COMM_H_
#define PCC_UART_COMM_H_

#include <msp430g2553.h>
#include <stdio.h>

//Call Before Any UART Communication
void Init_UART(void);

//Output Byte Via UART-USB
void OUTCHAR_UART(unsigned char);

//Input Byte via UART
unsigned char INCHAR_UART(void);

unsigned char currentChar(void);

char ASCII2Hex(char c);

char hex2ASCII(char c);

char toggleCase(char c);

void Init_LCD(void);//Not Used

void Hyperterminal_display(int);

void printStr(const char *str);
void printNewLine(void);
void println(const char *str);

//Used for printing numbers to terminal
typedef enum
{
	SIGNED_INT=0,
	UNSIGNED_INT,
	FLOAT,
	HEX
} NumberType;

//Print an integer to the terminal
void printInteger(unsigned long num);

#endif /* PCC_UART_COMM_H_ */
