// Copyright (c) Olaru Alexandru <olarualexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<math.h>

#include"serialW.h"

#define MAX_INPUT_LEN 64
#define KB 1024

int asciiZero = (int)'0', asciiNine = (int)'9';

int parseIntFromStr(const char *str)
{
	int integer = 0;

	for(int i = 0; i < strlen(str); ++i)
		if((int)str[i] >= asciiZero && (int)str[i] <= asciiNine)
			integer = integer * 10 + (int)str[i] - asciiZero;

	return integer;
}

int getUsedMemInMB(const char *totMem, const char *avalMem, char *bufferMem, char *cachedMem)
{
	int totMemInt = parseIntFromStr(totMem);
	int avalMemInt = parseIntFromStr(avalMem);
	int bufferMemInt = parseIntFromStr(bufferMem);
	int cachedMemInt = parseIntFromStr(cachedMem);

	return ((totMemInt - avalMemInt) - (bufferMemInt - cachedMemInt)) / KB;
}

void intToString(int integer, int integerLen, char *destStr)
{
	int aux = 1;
	
	while(aux <= integerLen)
	{
		destStr[aux - 1] = (char)(integer / (int)pow(10, integerLen - aux) + asciiZero);
		integer = integer % (int)pow(10, integerLen - aux++);
	}
	
	destStr[aux++ - 1] = 'X';
	destStr[aux - 1] = '\0';
}

int getIntLen(int integer)
{
	return (int)log10(integer) + 1;
}

int main(void)
{

#ifdef __linux__
	FILE *file;

	serialInit("ttyUSB0");

	while(1)
	{
		if(!(file = fopen("/proc/meminfo", "r")))
		{
			printf("Fatal error: meminfo was not found in /proc/\n");
			break;
		}

		char totMemField[MAX_INPUT_LEN], avalMemField[MAX_INPUT_LEN], buffersMemField[MAX_INPUT_LEN], cachedMemField[MAX_INPUT_LEN], line[MAX_INPUT_LEN];

		while(fgets(line, MAX_INPUT_LEN, file))
		{
			if(strstr(line, "MemTotal"))
				strcpy(totMemField, line);
			else if(strstr(line, "Buffers"))
				strcpy(buffersMemField, line);
			else if(strstr(line, "Cached"))
				strcpy(cachedMemField, line);
			else if(strstr(line, "MemAvailable"))
				strcpy(avalMemField, line);
		}

		int usedMemInt = getUsedMemInMB(totMemField, avalMemField, buffersMemField, cachedMemField);
		int usedMemIntLen = getIntLen(usedMemInt);
		char usedMemStr[usedMemIntLen + 1];
		intToString(usedMemInt, usedMemIntLen, usedMemStr);
		writeToSerial(usedMemStr);

		fclose(file);

		sleep(1);
	}

	serialClose();

#endif // __linux__

	return 0;
}
