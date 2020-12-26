// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#include"memDuino.h"
#include"serial.h"
#include<math.h>
#include"exitcodes.h"

#define SLEEP_INIT_TRY_MS 1500
#define MAGIC 0x1BADF14D

#if defined(__linux__)

#include<time.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define BUFF_LENGTH 64

typedef struct
{
    unsigned int total;
    unsigned int buffers;
    unsigned int cached;
    unsigned int free;
    unsigned int shared;
    unsigned int s_reclaimable;
    /* Last used mem value that was sent to the arduino */
    unsigned int used;
} MemInfo;

static unsigned int try_parse_uint(const char *str)
{
	unsigned int integer = 0;
	size_t i = 0;
	for(; i < strlen(str); ++i)
	{
		if((int)str[i] >= 48 && (int)str[i] <= 57)
		{
			integer = integer * 10 + (int)str[i] - 48;
		}
	}

	return integer;
}

static int str_starts_with(const char *str, const char *subStr)
{
	const unsigned int sub_str_len = strlen(subStr);

	if(sub_str_len > strlen(str))
	{
		return 0;
	}

	size_t i = 0;
	for(; i < sub_str_len; ++i)
	{
		if(str[i] != subStr[i])
		{
			return 0;
		}
	}

	return 1;
}

static void set_used_mem_mb(MemInfo *out_memInfo)
{
	// taken from https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290
	out_memInfo->used = (
		out_memInfo->total - 
		out_memInfo->free - (
			out_memInfo->buffers + (
				out_memInfo->cached + out_memInfo->s_reclaimable - out_memInfo->shared
			)
		)
	) / 1024;
}

#endif // __linux__

static void create_packet(char *out_packet, unsigned int used_mem, size_t used_mem_length)
{
	size_t i = 0;
	
	out_packet[i++] = 'S';

	while(i <= used_mem_length)
	{
		out_packet[i] = (char)(used_mem / (int)pow(10, used_mem_length - i) + 48);
		used_mem %= (int)pow(10, used_mem_length - i++);
	}

	out_packet[i] = 'E';
	/* start and end the packet with S and E respectively */
}

static void sleep_for_ms(time_t time)
{
#if defined(__linux__)
#if _POSIX_C_SOURCE > 199309L
	struct timespec ts;
	ts.tv_sec = time / 1000;
	ts.tv_nsec = (time % 1000) * 1000000;
	nanosleep(&ts, NULL);
#else
	usleep(time * 1000);
#endif // _POSIX_C_SOURCE
#elif defined(_WIN32)
	Sleep(time);
#endif //__linux__
}

int start_memduino(unsigned int update_interval_ms, unsigned int init_timeout_ms)
{
	unsigned int init_elapsed_time = 0;
	int device_fd;

	while(try_serial_init("ttyUSB0", &device_fd) != SERIAL_INIT_OK)
	{
		if(init_elapsed_time >= init_timeout_ms)
		{
			printf("Failed to init memDuino!\n");
			return TIMEOUT_SERIAL;
		}
		
		sleep_for_ms(SLEEP_INIT_TRY_MS);
		init_elapsed_time += SLEEP_INIT_TRY_MS;
	}

	size_t used_mem_length;
	char *packet;

#if defined(__linux__)

	FILE *file;
	char line[BUFF_LENGTH];
	MemInfo mem_info = {
		.total = 0,
		.buffers = 0,
		.cached = 0,
		.free = 0,
		.shared = 0,
		.s_reclaimable = 0,
		.used = 0
	};

	while(1)
	{
		if(!(file = fopen("/proc/meminfo", "r")))
		{
			printf("Fatal error: /proc/meminfo not found\n");
			serial_close(&device_fd);
			return MEMINFO_NOT_FOUND;
		}

		while(fgets(line, BUFF_LENGTH, file))
		{
			if(str_starts_with(line, "MemTotal"))
			{
				mem_info.total = try_parse_uint(line);
			}
			else if(str_starts_with(line, "Buffers"))
			{
				mem_info.buffers = try_parse_uint(line);
			}
			else if(str_starts_with(line, "Cached"))
			{
				mem_info.cached = try_parse_uint(line);
			}
			else if(str_starts_with(line, "MemFree"))
			{
				mem_info.free = try_parse_uint(line);
			}
			else if(str_starts_with(line, "Shmem"))
			{
				mem_info.shared = try_parse_uint(line);
			}
			else if(str_starts_with(line, "SReclaimable"))
			{
				mem_info.s_reclaimable = try_parse_uint(line);
			}
		}

		fclose(file);

		set_used_mem_mb(&mem_info);

		used_mem_length = (int)log10(mem_info.used) + 1;
		packet = malloc(used_mem_length + 2);

		create_packet(packet, mem_info.used, used_mem_length);

		write_to_serial(&device_fd, packet);

		free(packet);
		sleep_for_ms(update_interval_ms);
	}

#elif defined(_WIN32)

	/* Hide the console window */
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);

	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);

	while (1)
	{
		GlobalMemoryStatusEx(&statex);

		mem_info.used_mem = (statex.ullTotalPhys - statex.ullAvailPhys) / (1024 * 1024);

		size_t used_mem_dig_cnt = (int)log10(mem_info.used_mem) + 1;
		char* packet = malloc(used_mem_dig_cnt + 2);

		create_packet(packet, mem_info.used_mem, used_mem_dig_cnt);

		write_to_serial(memduino, packet);

		free(packet);
		sleep_for_ms(memduino->info.update_interval_ms);
	}

#endif // __linux__
	serial_close(&device_fd);

	return EXIT_OK;
}
