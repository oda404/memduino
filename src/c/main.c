// Copyright (c) Olaru Alexandru <olarualexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<math.h>
#include<unistd.h>

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

int getUsedMemInMB(const int *memTotal, const int *memFree, const int *buffers, const int *cached, const int *sReclaimable, const int *shmem)
{
	// taken from https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290
	return ((*memTotal - *memFree) - (*buffers + (*cached + *sReclaimable - *shmem))) / KB;
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
		
		int memTotal, buffers, cached, memFree, shmem, sReclaimable;
		char line[MAX_INPUT_LEN];

		while(fgets(line, MAX_INPUT_LEN, file))
		{
			if(strstr(line, "MemTotal:"))
				memTotal = parseIntFromStr(line);
			else if(strstr(line, "Buffers:"))
				buffers = parseIntFromStr(line);
			else if(strstr(line, "Cached:") && !strstr(line, "SwapCached:"))
				cached = parseIntFromStr(line);
			else if(strstr(line, "MemFree:"))
				memFree = parseIntFromStr(line);
			else if(strstr(line, "Shmem:"))
				shmem = parseIntFromStr(line);
			else if(strstr(line, "SReclaimable:"))
				sReclaimable = parseIntFromStr(line);
		}

		int usedMem = getUsedMemInMB(&memTotal, &memFree, &buffers, &cached, &sReclaimable, &shmem);
		int usedMemLen = getIntLen(usedMem);
		char usedMemStr[usedMemLen + 1];
		intToString(usedMem, usedMemLen, usedMemStr);

		writeToSerial(usedMemStr);

		fclose(file);

		sleep(1);
	}

	serialClose();
#endif // __linux__

	return 0;
}
