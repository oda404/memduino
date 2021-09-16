// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#include<memduinod/memduinod.h>
#include<memduinod/serial.h>
#include<memduinod/exitcodes.h>
#include<memduinod/meminfo.h>

#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdint.h>

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

	while((device_fd = serial_init(config->serial_port)) < 0)
	{
		if(init_elapsed_ms >= config->serial_init_timeout_ms)
		{
			printf("[memduinod] Timed out trying to init the serial port\n");
			return EXIT_SERIAL_ERR;
		}
		
		sleep_ms(INIT_TRY_SLEEP_MS);
		init_elapsed_ms += INIT_TRY_SLEEP_MS;
	}

	while(1)
	{
		int32_t usedmem = meminfo_get_used_mem(MEMINFO_MB);
		if(usedmem < 0)
			return EXIT_MEMINFO_ERR;

		size_t usedmem_digs = (int)log10(usedmem) + 1;
		char *packet = malloc(sizeof(char) * (usedmem_digs + 3));

		packet_create(packet, usedmem, usedmem_digs);

		serial_write(device_fd, packet);

		free(packet);
		sleep_ms(config->update_interval_ms);
	}
	serial_close(device_fd);

	return EXIT_OK;
}
