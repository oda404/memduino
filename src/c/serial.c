// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#include"serial.h"

#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#define SERIAL_INIT_OK 0

#if defined(__linux__)

#include<unistd.h>
#include<termios.h>
#include<fcntl.h> 
#include<errno.h>

#elif defined(_WIN32)

#include<windows.h>

#endif // __linux__

int try_serial_init(const char *device_name, int *out_device_fd)
{
#if defined(__linux__)

	char *devicePath;
	/* strlen("/dev/") == 5 */
	devicePath = malloc(5 + strlen(device_name) + 1);
	strcpy(devicePath, "/dev/");
	strcat(devicePath, device_name);

	*out_device_fd = open(devicePath, O_WRONLY /* O_RDWR */ | O_NOCTTY | O_SYNC);

	free(devicePath);

	if(*out_device_fd < 0)
	{
		printf("Error oppening /dev/%s: %s\n", device_name, strerror(errno));
		return errno;
	}

	struct termios tty;

	if(tcgetattr(*out_device_fd, &tty) != 0)
	{
		printf("Error from tcgetattr: %s\n", strerror(errno));
		return errno;
	}

	if(cfsetospeed(&tty, (speed_t)B9600) != 0)
	{
		printf("Error from cfsetospeed: %s\n", strerror(errno));
		return errno;
	}

	// if(cfsetispeed(&tty, (speed_t)B9600) != 0)
	// {
	// 	printf("Error from cfsetospeed: %s\n", strerror(errno));
	// 	return -1;
	// }

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8 bits per char

	tty.c_oflag = 0;

	tty.c_cflag &= ~(PARENB | PARODD); 			// no parity
	tty.c_cflag |= 0;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CRTSCTS;

	if(tcsetattr(*out_device_fd, TCSANOW, &tty) != 0)
	{
		printf("Error from tcsetattr: %s\n", strerror(errno));
		return errno;
	}

#elif defined(_WIN32)

	BOOL Status;

	memduino->info.commHandle = CreateFileA(memduino->info.device_name, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (memduino->info.commHandle == INVALID_HANDLE_VALUE)
	{
		printf("Error in opening serial port %s\n", memduino->info.device_name);
	}

	DCB dcb;
	dcb.DCBlength = sizeof(dcb);

	Status = GetCommState(memduino->info.commHandle, &dcb);
	if (!Status)
	{
		printf("Error in GetCommState\n");
	}

	/* Defaut config for arduinos */
	dcb.BaudRate = CBR_9600;	// 9600 Baud speed
	dcb.ByteSize = 8;			// 8 bits per byte
	dcb.StopBits = ONESTOPBIT;	// 1 stop bit
	dcb.Parity   = PARITY_NONE;	// no parity

	Status = SetCommState(memduino->info.commHandle, &dcb);

	if (!Status)
	{
		printf("Error in setting DCB Struct\n");
	}

	COMMTIMEOUTS timeouts; // port timeouts
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	Status = SetCommTimeouts(memduino->info.commHandle, &timeouts);

	if (!Status)
	{
		printf("Error in setting port timeouts\n"); 
	}

#endif //__linux__

	return SERIAL_INIT_OK;
}

void write_to_serial(const int *device_fd, const char *data)
{
#if defined(__linux__)
	size_t n = strlen(data);
	size_t written = write(*device_fd, data, n);
	if(written < n)
	{
		printf("Written %ld out of %ld\n", written, n);
	}

#elif defined(_WIN32)

	WriteFile(memduino->info.commHandle, data, strlen(data), NULL, NULL);

#endif //__linux__
}

void serial_close(const int *device_fd)
{
#if defined(__linux__)

	int status = close(*device_fd);
	if(status == -1)
	{
		printf("Error when closing %d: %s", *device_fd, strerror(errno));
	}

#elif defined(_WIN32)

	CloseHandle(memduino->info.commHandle);

#endif //__linux__
}
