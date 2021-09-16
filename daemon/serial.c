// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#include"serial.h"

#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#include<unistd.h>
#include<termios.h>
#include<fcntl.h> 
#include<errno.h>

int try_serial_init(const char *device_name, int *out_device_fd)
{
	*out_device_fd = open(device_name, O_WRONLY | O_NOCTTY | O_SYNC);

	if(*out_device_fd < 0)
	{
		printf("Error oppening %s: %s\n", device_name, strerror(errno));
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

	return SERIAL_INIT_OK;
}

void write_to_serial(const int *device_fd, const char *data)
{
	size_t n = strlen(data);
	size_t written = write(*device_fd, data, n);
	if(written < n)
	{
		printf("Written %ld out of %ld\n", written, n);
	}
}

void serial_close(const int *device_fd)
{
	int status = close(*device_fd);
	if(status == -1)
	{
		printf("Error when closing %d: %s", *device_fd, strerror(errno));
	}
}
