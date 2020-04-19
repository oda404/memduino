// Copyright (c) Olaru Alexandru <olarualexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.
#ifndef SERIAL_W
#define SERIAL_W

#include<string.h>
#include<unistd.h>
#include<fcntl.h> 
#include<termios.h>

/* definition of CRTSCTS */
#define CRTSCTS  020000000000 

int usb;
struct termios tty;

void serialInit(const char *);

void writeToSerial(const char *);

void serialClose(void);

#endif // SERIAL_W
