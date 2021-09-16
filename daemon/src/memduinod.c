// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#include<memduinod/memduinod.h>
#include<memduinod/serial.h>
#include<memduinod/exitcodes.h>

#include<time.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>
#include<stdint.h>

typedef struct
{
    unsigned int total;
    unsigned int buffers;
    unsigned int cached;
    unsigned int free;
    unsigned int shared;
    unsigned int s_reclaimable;
} MemInfo;

/**
 * Parses and extracts memory information from /proc/meminfo,
 * one line at a time.
*/
static void meminfo_parse_info(const char *line, MemInfo *meminfo)
{
	//TODO: check the actual unit (kB mB) ??
	if(strstr(line, "MemTotal:") == line)
		meminfo->total = strtoul(strchr(line, ':') + 1, NULL, 10);
	else if(strstr(line, "Buffers:") == line)
		meminfo->buffers = strtoul(strchr(line, ':') + 1, NULL, 10);
	else if(strstr(line, "Cached:") == line)
		meminfo->cached = strtoul(strchr(line, ':') + 1, NULL, 10);
	else if(strstr(line, "MemFree:") == line)
		meminfo->free = strtoul(strchr(line, ':') + 1, NULL, 10);
	else if(strstr(line, "Shmem:") == line)
		meminfo->shared = strtoul(strchr(line, ':') + 1, NULL, 10);
	else if(strstr(line, "SReclaimable:") == line)
		meminfo->s_reclaimable = strtoul(strchr(line, ':') + 1, NULL, 10);
}

/**
 * @returns Used memory in mB.
*/
static uint32_t meminfo_calculate_used_mem_mb(const MemInfo *meminfo)
{
	return (
		meminfo->total - meminfo->free - (
			meminfo->buffers + (
				meminfo->cached + meminfo->s_reclaimable - meminfo->shared
			)
		)
	) / 1024;
}

static void packet_create(
	char *packet, 
	unsigned int usedmem, 
	size_t usedmem_digs
)
{
	size_t i = 0;
	
	packet[i++] = 'S';

	while(i <= usedmem_digs)
	{
		packet[i] = (char)(usedmem / (int)pow(10, usedmem_digs - i) + '0');
		usedmem %= (int)pow(10, usedmem_digs - i++);
	}

	packet[i++] = 'E';
	packet[i] = '\0';
	/* start and end the packet with S and E respectively */
}

static void sleep_ms(time_t time)
{
#if defined(__linux__)
#if _POSIX_C_SOURCE > 199309L
	struct timespec ts;
	ts.tv_sec = time / 1000;
	ts.tv_nsec = (time % 1000) * 1000000;
	nanosleep(&ts, NULL);
#else
	usleep(time * 1000);
#endif // _POSIX_C_SOURCE > 199309L
#else
#	error "Unknown platform"
#endif // defined(__linux__)
}

#define INIT_TRY_SLEEP_MS 1000

int memduinod_start(
	const MemduinodConfig *config
)
{
	uint32_t init_elapsed_ms = 0;
	int device_fd;

	while(try_serial_init(config->serial_port, &device_fd) != SERIAL_INIT_OK)
	{
		if(init_elapsed_ms >= config->serial_init_timeout_ms)
		{
			printf("Timed out trying to init the serial port\n");
			return SERIAL_TIMEOUT;
		}
		
		sleep_ms(INIT_TRY_SLEEP_MS);
		init_elapsed_ms += INIT_TRY_SLEEP_MS;
	}

#define BUFF_LENGTH 64
	char line[BUFF_LENGTH + 1];

	MemInfo meminfo = {
		.total = 0,
		.buffers = 0,
		.cached = 0,
		.free = 0,
		.shared = 0,
		.s_reclaimable = 0
	};

	FILE *file = NULL;
	while(1)
	{
		if(!(file = fopen("/proc/meminfo", "r")))
		{
			printf("Fatal error: couldn't open /proc/meminfo\n");
			serial_close(&device_fd);
			return MEMINFO_OPEN_ERR;
		}

		while(fgets(line, BUFF_LENGTH, file))
			meminfo_parse_info(line, &meminfo);

		fclose(file);

		uint32_t usedmem = meminfo_calculate_used_mem_mb(&meminfo);
		size_t usedmem_digs = (int)log10(usedmem) + 1;
		char *packet = malloc(sizeof(char) * (usedmem_digs + 3));

		packet_create(packet, usedmem, usedmem_digs);

		write_to_serial(&device_fd, packet);

		free(packet);
		sleep_ms(config->update_interval_ms);
	}
	serial_close(&device_fd);

	return EXIT_OK;
}
