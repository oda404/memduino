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
