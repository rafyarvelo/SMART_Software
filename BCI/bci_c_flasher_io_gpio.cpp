#include "bci_c_flasher_io_gpio.h"

C_Flasher_IO_GPIO::C_Flasher_IO_GPIO()
{
    currGPIOVal = GPIO_LOW;
}

C_Flasher_IO_GPIO::~C_Flasher_IO_GPIO()
{
    /*
     * Disable GPIO pin
     */
    GPIOUnexport(GPIO_OUTPUT_PIN);
}

ConnectionStatusType C_Flasher_IO_GPIO::connect()
{
    /*
     * Enable GPIO pins
     */
    if (GPIO_FAILURE == GPIOExport(GPIO_OUTPUT_PIN))
    {
        return GPIO_FAILURE;
    }

    /*
     * Set GPIO directions
     */
    if (GPIO_FAILURE == GPIODirection(GPIO_OUTPUT_PIN, GPIO_OUT)) 
    {
        return GPIO_FAILURE;
    }


    connectionStatus = CONNECTED;
    return connectionStatus;
}

void C_Flasher_IO_GPIO::SendRVS()
{
    currGPIOVal = !currGPIOVal;
    //Write high to GPIO Pin
    GPIOWrite(GPIO_OUTPUT_PIN, currGPIOVal);
}

int C_Flasher_IO_GPIO::GPIOExport(int pin)
{
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;
 
	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (GPIO_FAILURE == fd)
    {
		debugLog->println(FLASHER_LOG, "Failed to open export for writing!", false, true);
		return GPIO_FAILURE;
	}
 
	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}
 
int C_Flasher_IO_GPIO::GPIOUnexport(int pin)
{
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;
 
	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (GPIO_FAILURE == fd) 
    {
		debugLog->println(FLASHER_LOG, "Failed to open unexport for writing!", false, true);
		return GPIO_FAILURE;
	}
 
	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}
 
int C_Flasher_IO_GPIO::GPIODirection(int pin, int dir)
{
	static const char s_directions_str[]  = "in\0out";
 
	char path[DIRECTION_MAX];
	int fd;
 
	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin, false, true);
	fd = open(path, O_WRONLY);
	if (GPIO_FAILURE == fd) 
    {
		debugLog->println(FLASHER_LOG, "Failed to open gpio direction for writing!", false, true);
		return GPIO_FAILURE;
	}
 
	if (GPIO_FAILURE == write(fd, &s_directions_str[GPIO_IN == dir ? 0 : 3], GPIO_IN == dir ? 2 : 3)) 
    {
		debugLog->println(FLASHER_LOG, "Failed to set direction!", false, true);
		return GPIO_FAILURE;
	}
 
	close(fd);
	return(0);
}
 
int C_Flasher_IO_GPIO::GPIORead(int pin)
{
	char path[VALUE_MAX];
	char value_str[3];
	int fd;
 
	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if (GPIO_FAILURE == fd) 
    {
		debugLog->println(FLASHER_LOG, "Failed to open gpio value for reading!", false, true);
		return GPIO_FAILURE;
	}
 
	if (GPIO_FAILURE == read(fd, value_str, 3)) 
    {
		debugLog->println(FLASHER_LOG, "Failed to read value!");
		return GPIO_FAILURE;
	}
 
	close(fd);
 
	return(atoi(value_str));
}
 
int C_Flasher_IO_GPIO::GPIOWrite(int pin, int value)
{
	static const char s_values_str[] = "01";
 
	char path[VALUE_MAX];
	int fd;
 
	snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (GPIO_FAILURE == fd) 
    {
		debugLog->println(FLASHER_LOG, "Failed to open gpio value for writing!", false, true);
		return GPIO_FAILURE;
	}
 
	if (1 != write(fd, &s_values_str[GPIO_LOW == value ? 0 : 1], 1)) 
    {
		debugLog->println(FLASHER_LOG, "Failed to write value!");
		return GPIO_FAILURE;
	}
 
	close(fd);
	return(0);
}
