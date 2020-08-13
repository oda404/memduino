// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

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
    int usedMem;
    // digit count in usedMem
    size_t usedMemLength;
    char *usedMemStr;
    int updateIntervalMs;
    char *deviceName;
} MemDuino;

// this will freeze the current thread
void startMemDuino(MemDuino *memDuino);

#endif // MEM_H