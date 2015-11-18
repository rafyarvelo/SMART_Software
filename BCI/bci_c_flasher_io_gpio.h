#ifndef C_FLASHER_IO_GPIO_H
#define C_FLASHER_IO_GPIO_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#ifndef WIN32
#include <sys/mman.h>
#endif

#include <unistd.h>
#include "bci_c_flasher_io.h"

// Access from ARM Running Linux
#define BCM2708_PERI_BASE    0x3F000000
#define GPIO_BASE            (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */
#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

#define GET_GPIO(g) (*(gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH

#define GPIO_PULL *(gpio+37) // Pull up/pull down
#define GPIO_PULLCLK0 *(gpio+38) // Pull up/pull down clock

class C_Flasher_IO_GPIO : public C_Flasher_IO, public C_Singleton<C_Flasher_IO_GPIO>
{
public:
    C_Flasher_IO_GPIO();
    virtual ~C_Flasher_IO_GPIO();

    virtual ConnectionStatusType connect();
    virtual void SendRVS(C_RVS* pRVS);

private:
    bool checkPin(int pin);
    void printButton(int g);
    ConnectionStatusType setup_io();

    //GPIO Map
    int  mem_fd;
    void *gpio_map;

    // I/O access
    volatile unsigned *gpio;
};

#endif // C_FLASHER_IO_GPIO_H
