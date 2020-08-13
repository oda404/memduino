// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#include"memDuino.h"

#ifdef __linux__

#define BUFF_LENGTH 64
    int memTotal;
    int buffers;
    int cached;
    int memFree;
    int shmem;
    int sReclaimable;

#elif _WIN32



#endif // __linux__

static int parseIntFromStr(const char *str)
{
	int integer = 0;

	for(size_t i = 0; i < strlen(str); ++i)
	{
		if((int)str[i] >= 48 && (int)str[i] <= 57)
		{
			integer = integer * 10 + (int)str[i] - 48;
		}
	}

	return integer;
}

static int strStartsWith(const char *targetStr, const char *subStr)
{
	size_t i = 0;

	for(; i < strlen(subStr); ++i)
	{
		if(targetStr[i] != subStr[i])
		{
			return 0;
		}
	}

	return 1;
}

static void createPacket(MemDuino *memDuino)
{
	size_t i = 0;
	
	memDuino->usedMemStr[i++] = 'S';

	while(i <= memDuino->usedMemLength)
	{
		memDuino->usedMemStr[i] = (char)(memDuino->usedMem / (int)pow(10, memDuino->usedMemLength - i) + 48);
		memDuino->usedMem = memDuino->usedMem % (int)pow(10, memDuino->usedMemLength - i++);
	}

	memDuino->usedMemStr[i++] = 'E';
	memDuino->usedMemStr[i++] = '\0';
	/* start and end the packet with S and E respectively */
}

static int getUsedMemInMB()
{
	// taken from https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290
	return ((memTotal - memFree) - (buffers + (cached + sReclaimable - shmem))) / 1024;
}

void startMemDuino(MemDuino *memDuino)
{
#ifdef __linux__

    FILE *file;
    char line[BUFF_LENGTH];

    if(serialInit(memDuino->deviceName) < 0)
	{
		return;
	}

    while(1)
    {
        if(!(file = fopen("/proc/meminfo", "r")))
		{
			printf("Fatal error: meminfo was not found in /proc/\n");
			break;
		}

        while(fgets(line, BUFF_LENGTH, file))
		{
			if(strStartsWith(line, "MemTotal"))
			{
				memTotal = parseIntFromStr(line);
			}
			else if(strStartsWith(line, "Buffers"))
			{
				buffers = parseIntFromStr(line);
			}
			else if(strStartsWith(line, "Cached"))
			{
				cached = parseIntFromStr(line);
			}
			else if(strStartsWith(line, "MemFree"))
			{
				memFree = parseIntFromStr(line);
			}
			else if(strStartsWith(line, "Shmem"))
			{
				shmem = parseIntFromStr(line);
			}
			else if(strStartsWith(line, "SReclaimable"))
			{
				sReclaimable = parseIntFromStr(line);
			}
        }

        fclose(file);

        memDuino->usedMem = getUsedMemInMB();
		memDuino->usedMemLength = (int)log10(memDuino->usedMem) + 1;
        memDuino->usedMemStr = malloc(memDuino->usedMemLength + 3);
        
        createPacket(memDuino);
        
        writeToSerial(memDuino->usedMemStr);

        free(memDuino->usedMemStr);
        sleep(memDuino->updateIntervalMs / 1000);
    }

    serialClose();

#elif _WIN32



#endif // __linux__
}
