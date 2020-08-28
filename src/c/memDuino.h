// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#ifndef MEM_DUINO_H
#define MEM_DUINO_H

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
} memduino_mem_info;

typedef struct
{
    /* Interval on which the packets are sent to the arduino */
    unsigned int update_interval_ms;
    /* Each init try takes 1000 ms */
    unsigned int init_timeout_ms;
    /* Name of the device to open */
    char *device_name;
    /* fd of the opened device */
    int device_fd;
} memduino_info;


typedef struct
{
    memduino_mem_info mem_info;
    memduino_info info;
} memduino;

/* this will freeze the current thread */
void start_memduino(memduino *memduino);

/* this spawns another thread */
void start_memduino_async(memduino *memduino);

#endif // MEM_DUINO_H
