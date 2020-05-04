// Copyright (c) Olaru Alexandru <olarualexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#include"serialW.h"

void serialInit(const char *deviceName)
{
#ifdef __linux__

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

	/* Default config for arduinos */
	cfsetospeed (&tty, (speed_t)B9600);			// baud speed of 9600
	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8 bits per byte
	tty.c_cflag &= ~(PARENB | PARODD); 			// no parity

	cfmakeraw(&tty);

	tcsetattr(usb, TCSANOW, &tty);

#elif _WIN32

	BOOL Status;

	hComm = CreateFileA(deviceName, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hComm == INVALID_HANDLE_VALUE)
		printf("Error in opening serial port %s\n", deviceName);

	dcb.DCBlength = sizeof(dcb);

	Status = GetCommState(hComm, &dcb);
	if (!Status)
		printf("Error in GetCommState\n");

	/* Defaut config for arduinos */
	dcb.BaudRate = CBR_9600;	// 9600 Baud speed
	dcb.ByteSize = 8;			// 8 bits per byte
	dcb.StopBits = ONESTOPBIT;	// 1 stop bit
	dcb.Parity   = PARITY_NONE;	// no parity

	Status = SetCommState(hComm, &dcb);

	if (!Status)
		printf("Error in setting DCB Struct\n");

	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	Status = SetCommTimeouts(hComm, &timeouts);

	if (!Status)
		printf("Error in setting port timeouts\n");

#endif
}

void writeToSerial(const char *data)
{
#ifdef __linux__

	write(usb, data, strlen(data));

#elif _WIN32

	WriteFile(hComm, data, strlen(data), NULL, NULL);

#endif
}

void serialClose(void)
{
#ifdef __linux__

	close(usb);

#elif _WIN32

	CloseHandle(hComm);

#endif
}
