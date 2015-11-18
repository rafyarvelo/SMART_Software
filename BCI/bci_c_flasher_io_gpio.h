#ifndef C_FLASHER_IO_GPIO_H
#define C_FLASHER_IO_GPIO_H

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "bci_c_flasher_io.h"

#define GPIO_IN  0
#define GPIO_OUT 1

#define GPIO_LOW  0
#define GPIO_HIGH 1

#define GPIO_INPUT_PIN  24 /* P1-18 */
#define GPIO_OUTPUT_PIN 4  /* P1-07 */

#define BUFFER_MAX 3
#define DIRECTION_MAX 35
#define VALUE_MAX 30

#define GPIO_FAILURE -1

//Communicate with Flasher Board through GPIO Pins on Raspberry Pi
class C_Flasher_IO_GPIO : public C_Flasher_IO, public C_Singleton<C_Flasher_IO_GPIO>
{
public:
    C_Flasher_IO_GPIO();
    virtual ~C_Flasher_IO_GPIO();

    virtual ConnectionStatusType connect();
    virtual void SendRVS(C_RVS* pRVS);

private:
    int GPIOExport(int pin);
    int GPIOUnexport(int pin);
    int GPIODirection(int pin, int dir);
    int GPIORead(int pin); 
    int GPIOWrite(int pin, int value);
};

#endif // C_FLASHER_IO_GPIO_H
