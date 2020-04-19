#include"serialW.h"

void serialInit(const char *deviceName)
{
    usb = open("/dev/ttyUSB0", O_RDWR| O_NOCTTY);
    memset (&tty, 0, sizeof tty);
    cfsetispeed (&tty, (speed_t)B9600);
    
    /* attributes for the port */
    tty.c_cflag     &=  ~PARENB;
	tty.c_cflag     &=  ~CSTOPB;
	tty.c_cflag     &=  ~CSIZE;
	tty.c_cflag     |=  CS8;
	tty.c_cflag     &=  ~CRTSCTS;
	tty.c_cc[VMIN]   =  1;
	tty.c_cc[VTIME]  =  5;
	tty.c_cflag     |=  CREAD | CLOCAL;

	cfmakeraw(&tty);

	tcflush(usb, TCIFLUSH);
}

void writeToSerial(const char *data)
{
	int n_written = 0, spot = 0;

	do {
		n_written = write(usb, &data[spot], 1);
		spot += n_written;
	} while (data[spot - 1] != '\0' && n_written > 0);
}

void serialClose(void)
{
    close(usb);
}
