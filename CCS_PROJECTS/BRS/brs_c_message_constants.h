/*
 * brs_c_message_constants.h
 *
 *  Created on: Sep 25, 2015
 *      Author: Rafael
 */

#ifndef BRS_C_MESSAGE_CONSTANTS_H_
#define BRS_C_MESSAGE_CONSTANTS_H_

/*
 * General Message Format:
 * MESSAGE ID - 4 Bytes Unsigned
 * MESSAGE SIZE - 4 Bytes Unsigned
 * ...
 * N Bytes of Data
 *
 */

#define MAX_BUFFER_SIZE 1024 //How many Bytes we will store at a maximum (1KB)

//Constants for Communication between Devices
#define MSG_ID_SIZE 4 //Bytes

//BRS Frame to the BCI
#define BRS2BCI_MSG_ID 0x123FEDEF

//BCI TM Frame Back to the BRS
#define BCI2BRS_MSG_ID 0xFEDEF321

//BRS to Mobile Device Communication
#define BRS2MD_MSG_ID 0x456ABCFD

//Mobile Device to BRS Communication
#define MD2BRS_MSG_ID 0xDFCBA654

#endif /* BRS_C_MESSAGE_CONSTANTS_H_ */
