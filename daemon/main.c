// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#include"memDuino.h"
#include"args.h"
#include"exitcodes.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

#define DEFAULT_UPDATE_INTERVAL_MS 1000
#define DEFAULT_INIT_TIMEOUT_MS 10000
#define DEFAULT_INIT_TRY_SLEEP_MS 1500
#define DEFAULT_SERIAL_PORT "/dev/ttyUSB0"

int main(int argc, char **argv)
{
	if(check_for_help_arg(argc, argv) == ARG_OK) return 0;

	/* init all of them to their default values */
	UI update_interval_ms = DEFAULT_UPDATE_INTERVAL_MS;
	UI init_timeout_ms = DEFAULT_INIT_TIMEOUT_MS;
	UI init_try_sleep_ms = DEFAULT_INIT_TRY_SLEEP_MS;

	char *serial_port = 
	malloc(sizeof(char) * (strlen(DEFAULT_SERIAL_PORT) + 1));
	strcpy(serial_port, DEFAULT_SERIAL_PORT);

	if(get_update_interval_arg(
		argc, 
		argv, 
		&update_interval_ms
	) == ARG_BAD) return BAD_ARG_FOUND;

	if(get_init_timeout_arg(
		argc, 
		argv, 
		&init_timeout_ms
	) == ARG_BAD) return BAD_ARG_FOUND;

	if(get_init_try_sleep_arg(
		argc, 
		argv, 
		&init_try_sleep_ms
	) == ARG_BAD) return BAD_ARG_FOUND;

	if(get_serial_port_arg(
		argc,
		argv,
		serial_port
	) == ARG_BAD) return BAD_ARG_FOUND;

	if(init_timeout_ms < init_try_sleep_ms)
	{
		printf(
			"Fatal error: init_timeout [%d] is less than init_try_sleep [%d]\n", 
			init_timeout_ms, 
			init_try_sleep_ms
		);
		return BAD_ARG_FOUND;
	}
	
	return start_memduino(
		update_interval_ms,
		init_timeout_ms,
		init_try_sleep_ms,
		serial_port
	);
}
