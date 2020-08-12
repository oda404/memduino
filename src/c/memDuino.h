#ifndef MEM_H
#define MEM_H

#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"serialW.h"

typedef struct
{
#ifdef __linux__
#define BUFF_LENGTH 64
    int memTotal;
    int buffers;
    int cached;
    int memFree;
    int shmem;
    int sReclaimable;
#endif // __linux__

    int usedMem;
    size_t usedMemLength;
    char *usedMemStr;

    int updateIntervalMs;
} MemDuino;

int getUsedMemInMB(MemDuino *memDuino);

// this will freeze the current thread
void startMemDuino(MemDuino *memDuino);

#endif // MEM_H