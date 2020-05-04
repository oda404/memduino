// Copyright (c) Olaru Alexandru <olarualexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.
#ifndef SERIAL_W
#define SERIAL_W

#include<string.h>
#include<fcntl.h> 
#include<stdio.h>

#ifdef __linux__

#include<unistd.h>
#include<terminos.h>

#elif _WIN32

#include<windows.h>

#endif


#ifdef __linux__

int    usb;
struct termios tty;

#elif _WIN32

HANDLE       hComm;    // serial port handle
DCB          dcb;      // port attributes
COMMTIMEOUTS timeouts; // port timeouts

#endif

void serialInit(const char *);

void writeToSerial(const char *);

void serialClose(void);

#endif // SERIAL_W
