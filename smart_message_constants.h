/*
 * smart_message_constants.h
 *
 *  Created on: Sep 25, 2015
 *      Author: Rafael
 */

#ifndef SMART_MESSAGE_CONSTANTS_H_
#define SMART_MESSAGE_CONSTANTS_H_

/*
 * General Message Format:
 * MESSAGE ID - 4 Bytes Unsigned
 * ...
 * N Bytes of Data
 *
 */

//4 Byte Message ID
//typedef unsigned int MSG_ID_Type;

//5 Byte Message ID
typedef char* MSG_ID_Type;

//Constants for Communication between Devices
#define MSG_ID_SIZE 5 //Bytes

#define MAX_BUFFER_SIZE 1024 //How many Bytes we will store at a maximum (1KB)

//BRS Frame to the BCI
#define BRS2BCI_MSG_ID "BRS!"

//BCI TM Frame Back to the BRS
#define BCI2BRS_MSG_ID "BCI!"

//BRS to Mobile Device Communication
#define BRS2MD_MSG_ID  "!BLT"

//Mobile Device to BRS Communication
#define MD2BRS_MSG_ID  "TLB!"

#endif /* SMART_MESSAGE_CONSTANTS_H_ */
