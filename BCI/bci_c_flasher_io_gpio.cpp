#include "bci_c_flasher_io_gpio.h"

C_Flasher_IO_GPIO::C_Flasher_IO_GPIO()
{

}

C_Flasher_IO_GPIO::~C_Flasher_IO_GPIO()
{

}

ConnectionStatusType C_Flasher_IO_GPIO::connect()
{
    return setup_io();
}

void C_Flasher_IO_GPIO::SendRVS(C_RVS *pRVS)
{

}

ConnectionStatusType C_Flasher_IO_GPIO::setup_io()
{
    connectionStatus = NOT_CONNECTED;
#ifndef WIN32
   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0)
   {
      debugLog->println(FLASHER_LOG, "Can't open /dev/mem", false, true);
      return NOT_CONNECTED;
   }

   /* mmap GPIO */
   gpio_map = mmap(
      NULL,             //Any adddress in our space will do
      BLOCK_SIZE,       //Map length
      PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
      MAP_SHARED,       //Shared with other processes
      mem_fd,           //File to map
      GPIO_BASE         //Offset to GPIO peripheral
   );

   close(mem_fd); //No need to keep mem_fd open after mmap

   if (gpio_map == MAP_FAILED) {
      debugLog->println(FLASHER_LOG, "mmap error %d\n", (int)gpio_map);//errno also set!
      return NOT_CONNECTED;
   }

   // Always use volatile pointer!
   gpio = (volatile unsigned *)gpio_map;
#endif

   connectionStatus = CONNECTED;
   return connectionStatus;
} // setup_io

bool C_Flasher_IO_GPIO::checkPin(int pin)
{
    return (bool) GET_GPIO(pin);
}

void C_Flasher_IO_GPIO::printButton(int g)
{
  if (GET_GPIO(g)) // !=0 <-> bit is 1 <- port is HIGH=3.3V
    debugLog->println(FLASHER_LOG, "Button pressed!\n");
  else // port is LOW=0V
    debugLog->println(FLASHER_LOG, "Button released!\n");
}

