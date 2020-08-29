// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#include"serial.h"

#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#ifdef __linux__

#include<unistd.h>
#include<termios.h>
#include<fcntl.h> 
#include<errno.h>

#elif _WIN32

#include<windows.h>

#endif // __linux__

int serial_init(memduino *md)
{
#ifdef __linux__

	/* strlen("/dev/") == 5 */
	char *devicePath;
	devicePath = malloc(5 + strlen(md->info.device_name) + 1);
	strcpy(devicePath, "/dev/");
	strcat(devicePath, md->info.device_name);

	md->info.device_fd = open(devicePath, O_WRONLY | O_NOCTTY | O_SYNC);

	free(devicePath);

	if(md->info.device_fd < 0)
	{
		printf("Error when oppening /dev/%s: %s\n", md->info.device_name, strerror(errno));
		return -1;
	}

	struct termios tty;

	if(tcgetattr(md->info.device_fd, &tty) != 0)
	{
		printf("Error from tcgetattr: %s\n", strerror(errno));
		return -1;
	}

	if(cfsetospeed(&tty, (speed_t)B9600) != 0)
	{
		printf("Error from cfsetospeed: %s\n", strerror(errno));
		return -1;
	}

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8 bits per char
	
	tty.c_oflag = 0;

	tty.c_cflag &= ~(PARENB | PARODD); 			// no parity
	tty.c_cflag |= 0;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CRTSCTS;

	if(tcsetattr(md->info.device_fd, TCSANOW, &tty) != 0)
	{
		printf("Error from tcsetattr: %s\n", strerror(errno));
		return -1;
	}

#elif _WIN32

	BOOL Status;

	md->info.commHandle = CreateFileA(md->info.device_name, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (md->info.commHandle == INVALID_HANDLE_VALUE)
	{
		printf("Error in opening serial port %s\n", md->info.device_name);
	}

	DCB dcb;
	dcb.DCBlength = sizeof(dcb);

	Status = GetCommState(md->info.commHandle, &dcb);
	if (!Status)
	{
		printf("Error in GetCommState\n");
	}

	/* Defaut config for arduinos */
	dcb.BaudRate = CBR_9600;	// 9600 Baud speed
	dcb.ByteSize = 8;			// 8 bits per byte
	dcb.StopBits = ONESTOPBIT;	// 1 stop bit
	dcb.Parity   = PARITY_NONE;	// no parity

	Status = SetCommState(md->info.commHandle, &dcb);

	if (!Status)
	{
		printf("Error in setting DCB Struct\n");
	}

	COMMTIMEOUTS timeouts; // port timeouts
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	Status = SetCommTimeouts(md->info.commHandle, &timeouts);

	if (!Status)
	{
		printf("Error in setting port timeouts\n"); 
	}

#endif

	return 0;
}

void write_to_serial(memduino *md, const char *data)
{
#ifdef __linux__
	size_t n = strlen(data);
	size_t written = write(md->info.device_fd, data, n);
	if(written < n)
	{
		printf("Written %ld out of %ld\n", written, n);
	}

#elif _WIN32

	WriteFile(md->info.commHandle, data, strlen(data), NULL, NULL);

#endif
}

void serial_close(memduino *md)
{
#ifdef __linux__

	int status = close(md->info.device_fd);
	if(status == -1)
	{
		printf("Error when closing %d: %s", fd, strerror(errno));
	}

#elif _WIN32

	CloseHandle(md->info.commHandle);

#endif
}
