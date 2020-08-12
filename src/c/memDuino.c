#include"memDuino.h"

static int strToInt(const char *str)
{
	int integer = 0;

	for(int i = 0; i < strlen(str); ++i)
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
	int aux = 1;
	int i = 0;
	
	memDuino->usedMemStr[i++] = 'S';

	while(aux <= memDuino->usedMemLength)
	{
		memDuino->usedMemStr[i++] = (char)(memDuino->usedMem / (int)pow(10, memDuino->usedMemLength - aux) + 48);
		memDuino->usedMem = memDuino->usedMem % (int)pow(10, memDuino->usedMemLength - aux++);
	}

	memDuino->usedMemStr[i++] = 'E';
	
	memDuino->usedMemStr[i++] = '\0';

	/* start and end the packet with S and E respectively */
}

int getUsedMemInMB(MemDuino *memDuino)
{
	// taken from https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290
	return ((memDuino->memTotal - memDuino->memFree) - (memDuino->buffers + (memDuino->cached + memDuino->sReclaimable - memDuino->shmem))) / 1024;
}

void startMemDuino(MemDuino *memDuino)
{
    FILE *file;
    char line[BUFF_LENGTH];

    serialInit("ttyUSB0");

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
				memDuino->memTotal = strToInt(line);
			}
			else if(strStartsWith(line, "Buffers"))
			{
				memDuino->buffers = strToInt(line);
			}
			else if(strStartsWith(line, "Cached"))
			{
				memDuino->cached = strToInt(line);
			}
			else if(strStartsWith(line, "MemFree"))
			{
				memDuino->memFree = strToInt(line);
			}
			else if(strStartsWith(line, "Shmem"))
			{
				memDuino->shmem = strToInt(line);
			}
			else if(strStartsWith(line, "SReclaimable"))
			{
				memDuino->sReclaimable = strToInt(line);
			}
        }

        fclose(file);

        memDuino->usedMem = getUsedMemInMB(memDuino);
		memDuino->usedMemLength = (int)log10(memDuino->usedMem) + 1;
        memDuino->usedMemStr = malloc(memDuino->usedMemLength + 3);
        
        createPacket(memDuino);
        
        writeToSerial(memDuino->usedMemStr);

        free(memDuino->usedMemStr);
        sleep(memDuino->updateIntervalMs / 1000);
    }

    serialClose();
}
