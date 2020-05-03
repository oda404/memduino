// Copyright (c) Olaru Alexandru <olarualexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#include"serialW.h"

void serialInit(const char *deviceName)
{
	/* strlen("/dev/") == 5 */
	char devicePath[5 + strlen(deviceName) + 1];
	strcpy(devicePath, "/dev/");
	strcat(devicePath, deviceName);

	usb = open(devicePath, O_WRONLY| O_NOCTTY | O_SYNC);

	if(usb < 0)
	{
		printf("Error in opening %s\n", deviceName);
	}

	memset(&tty, 0, sizeof tty);

	tcgetattr(usb, &tty);

	cfsetospeed (&tty, (speed_t)B9600);			// baud speed of 9600

	/* attributes for the port */
	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8 bits per byte
	tty.c_cflag &= ~(PARENB | PARODD); 			// no parity

	cfmakeraw(&tty);

	tcsetattr(usb, TCSANOW, &tty);
}

void writeToSerial(const char *data)
{
	write(usb, data, strlen(data));
}

void serialClose(void)
{
	close(usb);
}
