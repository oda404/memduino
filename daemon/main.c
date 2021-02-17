// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#include"memDuino.h"
#include"args.h"
#include"exitcodes.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

#define ARGS_CNT                  5

#define HELP_ARG                  "help"
#define HELP_ARG_SHORT            "-h"
#define HELP_ARG_LONG             "--help"

#define UPDATE_INTERVAL_ARG       "update_interval"
#define UPDATE_INTERVAL_ARG_SHORT "-u"
#define UPDATE_INTERVAL_ARG_LONG  "--update-interval"

#define INIT_TIMEOUT_ARG          "init_timeout"
#define INIT_TIMEOUT_ARG_SHORT    "-i"
#define INIT_TIMEOUT_ARG_LONG     "--init-timeout"

#define INIT_TRY_SLEEP_ARG        "init_try_sleep"
#define INIT_TRY_SLEEP_ARG_SHORT  "-I"
#define INIT_TRY_SLEEP_ARG_LONG   "--init-try-sleep"

#define SERIAL_PORT_ARG           "serial_port"
#define SERIAL_PORT_ARG_SHORT     "-s"
#define SERIAL_PORT_ARG_LONG      "--serial-port"

#define DEFAULT_UPDATE_INTERVAL_MS 1000
#define DEFAULT_INIT_TIMEOUT_MS    10000
#define DEFAULT_INIT_TRY_SLEEP_MS  1500
#define DEFAULT_SERIAL_PORT        "/dev/ttyUSB0"

int main(int argc, char **argv)
{
	Args args;
	args_init(&args);

	args_add(HELP_ARG, HELP_ARG_SHORT, HELP_ARG_LONG, true, &args);
	args_add(UPDATE_INTERVAL_ARG, UPDATE_INTERVAL_ARG_SHORT, UPDATE_INTERVAL_ARG_LONG, false, &args);
	args_add(INIT_TIMEOUT_ARG, INIT_TIMEOUT_ARG_SHORT, INIT_TIMEOUT_ARG_LONG, false, &args);
	args_add(INIT_TRY_SLEEP_ARG, INIT_TRY_SLEEP_ARG_SHORT, INIT_TRY_SLEEP_ARG_LONG, false, &args);
	args_add(SERIAL_PORT_ARG, SERIAL_PORT_ARG_SHORT, SERIAL_PORT_ARG_LONG, false, &args);

	args_parse(argc, argv, &args);

	/* init all of them to their default values */
	UI update_interval_ms = DEFAULT_UPDATE_INTERVAL_MS;
	UI init_timeout_ms = DEFAULT_INIT_TIMEOUT_MS;
	UI init_try_sleep_ms = DEFAULT_INIT_TRY_SLEEP_MS;
	char *serial_port = 
	malloc(sizeof(char) * (strlen(DEFAULT_SERIAL_PORT) + 1));
	strcpy(serial_port, DEFAULT_SERIAL_PORT);

	if(args_get_bool(HELP_ARG, &args))
	{
		print_help_message();
		return EXIT_OK;
	}

	args_out_uint(UPDATE_INTERVAL_ARG, &update_interval_ms, &args);
	args_out_uint(INIT_TIMEOUT_ARG, &init_timeout_ms, &args);
	args_out_uint(INIT_TRY_SLEEP_ARG, &init_try_sleep_ms, &args);
	args_out_str(SERIAL_PORT_ARG, &serial_port, &args);

	if(init_try_sleep_ms > init_timeout_ms)
	{
		printf("Fatal error: init-try-sleep is bigger than init-timeout\n");
		return BAD_ARG_FOUND;
	}

	printf("%s\n", serial_port);
	
	int status = start_memduino(
		update_interval_ms,
		init_timeout_ms,
		init_try_sleep_ms,
		serial_port
	);

	args_free(&args);
	free(serial_port);

	return status;
}
