// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#include "memDuino.h"
#include "exitcodes.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <argx/argx.h>

#define HELP_ARG "help"
#define HELP_ARG_SHORT "-h"
#define HELP_ARG_LONG "--help"

#define UPDATE_INTERVAL_ARG "update_interval"
#define UPDATE_INTERVAL_ARG_SHORT "-u"
#define UPDATE_INTERVAL_ARG_LONG "--update-interval"

#define INIT_TIMEOUT_ARG "init_timeout"
#define INIT_TIMEOUT_ARG_SHORT "-i"
#define INIT_TIMEOUT_ARG_LONG "--init-timeout"

#define SERIAL_PORT_ARG "serial_port"
#define SERIAL_PORT_ARG_SHORT "-s"
#define SERIAL_PORT_ARG_LONG "--serial-port"

#define DUMP_DEFAULTS_ARG "dump_defaults"
#define DUMP_DEFAULTS_ARG_SHORT "-d"
#define DUMP_DEFAULTS_ARG_LONG "--dump-defaults"

#define DEFAULT_UPDATE_INTERVAL_MS 1000
#define DEFAULT_INIT_TIMEOUT_MS 10000
#define DEFAULT_INIT_TRY_SLEEP_MS 1500
#define DEFAULT_SERIAL_PORT "/dev/ttyUSB0"

int main(int argc, char **argv)
{
	Argx argx;
	argx_init(&argx);

	argx_arg_add(
		HELP_ARG,
		HELP_ARG_SHORT,
		HELP_ARG_LONG,
		"Show this message and exit.",
		true,
		&argx
	);
	argx_arg_add(
		UPDATE_INTERVAL_ARG,
		UPDATE_INTERVAL_ARG_SHORT,
		UPDATE_INTERVAL_ARG_LONG,
		"Interval at which data packets are sent to the Arduino in milliseconds.",
		false,
		&argx
	);
	argx_arg_add(
		INIT_TIMEOUT_ARG,
		INIT_TIMEOUT_ARG_SHORT,
		INIT_TIMEOUT_ARG_LONG,
		"Timeout for connecting to the serial port in milliseconds.",
		false,
		&argx
	);
	argx_arg_add(
		SERIAL_PORT_ARG,
		SERIAL_PORT_ARG_SHORT,
		SERIAL_PORT_ARG_LONG,
		"Serial port to which the Arduino is connected.",
		false,
		&argx
	);
	argx_arg_add(
		DUMP_DEFAULTS_ARG,
		DUMP_DEFAULTS_ARG_SHORT,
		DUMP_DEFAULTS_ARG_LONG,
		"For every available argument, dump it's default value to stdout.",
		true,
		&argx
	);
	argx_help_msg_gen("memduinod [options]", "Starts the memduino daemon.", &argx);

	argx_args_parse(argv, argc, &argx);

	if (argx_arg_present(HELP_ARG, &argx))
	{
		printf("%s,", argx_help_msg_get(&argx));
		return EXIT_OK;
	}

	if (argx_arg_present(DUMP_DEFAULTS_ARG, &argx))
	{
		printf("Argument defaults:\n");
		printf("  %s,%s: %dms\n", UPDATE_INTERVAL_ARG_SHORT, UPDATE_INTERVAL_ARG_LONG, DEFAULT_UPDATE_INTERVAL_MS);
		printf("  %s,%s: %d\n", INIT_TIMEOUT_ARG_SHORT, INIT_TIMEOUT_ARG_LONG, DEFAULT_INIT_TIMEOUT_MS);
		printf("  %s,%s: %s\n", SERIAL_PORT_ARG_SHORT, SERIAL_PORT_ARG_LONG, DEFAULT_SERIAL_PORT);
		return EXIT_OK;
	}

	unsigned int
		update_interval_ms = 0,
		serial_init_timeout_ms = 0;
	char *serial_port = NULL;

	if (argx_arg_present(UPDATE_INTERVAL_ARG, &argx))
		argx_arg_val_get_uint(UPDATE_INTERVAL_ARG, &update_interval_ms, &argx);
	else
		update_interval_ms = DEFAULT_UPDATE_INTERVAL_MS;

	if (argx_arg_present(INIT_TIMEOUT_ARG, &argx))
		argx_arg_val_get_uint(INIT_TIMEOUT_ARG, &serial_init_timeout_ms, &argx);
	else
		serial_init_timeout_ms = DEFAULT_INIT_TIMEOUT_MS;

	if (argx_arg_present(SERIAL_PORT_ARG, &argx))
	{
		size_t size;
		argx_arg_val_get_strlen(SERIAL_PORT_ARG, &size, &argx);
		serial_port = malloc(size + 1);
		argx_arg_val_get_str(SERIAL_PORT_ARG, serial_port, &argx);
	}
	else
	{
		serial_port = malloc(strlen(DEFAULT_SERIAL_PORT) + 1);
		strcpy(serial_port, DEFAULT_SERIAL_PORT);
	}

	argx_destroy(&argx);

	MemduinodConfig config = {
		.serial_port = serial_port,
		.serial_init_timeout_ms = serial_init_timeout_ms,
		.update_interval_ms = update_interval_ms
	};

	int status = memduinod_start(&config);

	free(serial_port);

	return status;
}
