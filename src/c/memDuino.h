// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#ifndef MEMDUINO_H
#define MEMDUINO_H

#if defined(_WIN32)

#include<Windows.h>

#endif // _WIN32

typedef struct
{
    unsigned int mem_total;
    unsigned int buffers;
    unsigned int cached;
    unsigned int mem_free;
    unsigned int sh_mem;
    unsigned int s_reclaimable;
    /* Last used mem value that was sent to the arduino */
    unsigned int used_mem;
} MemDuinoMemInfo;

typedef struct
{
    /* Interval on which the packets are sent to the arduino */
    unsigned int update_interval_ms;
    /* Each init try takes 1000 ms */
    unsigned int init_timeout_ms;
    /* Name of the device to open */
    char *device_name;
#if defined(__linux__)
    /* fd of the opened device */
    int device_fd;
#elif defined(_WIN32)
    HANDLE commHandle;
#endif // __linux__
} MemDuinoInfo;


typedef struct
{
    MemDuinoMemInfo mem_info;
    MemDuinoInfo info;
} MemDuino;

/* this will freeze the current thread */
void start_memduino(MemDuino *memduino);

/* this spawns another thread */
void start_memduino_async(MemDuino *memduino);

#endif // MEMDUINO_H
