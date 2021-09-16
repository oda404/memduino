// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#include <memduinod/memduinod.h>
#include <memduinod/exitcodes.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <argx/argx.h>

#define STRINGIFY(x) #x
#define TOSTR(x) STRINGIFY(x)

#define HELP_ARG "help"
#define HELP_ARG_SHORT "-h"
#define HELP_ARG_LONG "--help"
#define HELP_ARG_DESC "Show this message and exit."

#define UPDATE_INTERVAL_ARG "update_interval"
#define UPDATE_INTERVAL_ARG_SHORT "-u"
#define UPDATE_INTERVAL_ARG_LONG "--update-interval <val>"
#define DEFAULT_UPDATE_INTERVAL_MS 1000
#define UPDATE_INTERVAL_ARG_DESC "Interval at which data packets are sent to the Arduino in milliseconds. (default: " TOSTR(DEFAULT_UPDATE_INTERVAL_MS) ")"

#define INIT_TIMEOUT_ARG "init_timeout"
#define INIT_TIMEOUT_ARG_SHORT "-i"
#define INIT_TIMEOUT_ARG_LONG "--init-timeout <val>"
#define DEFAULT_INIT_TIMEOUT_MS 10000
#define INIT_TIMEOUT_ARG_DESC "Timeout for connecting to the serial port in milliseconds. (default: " TOSTR(DEFAULT_INIT_TIMEOUT_MS) ")"

#define SERIAL_PORT_ARG "serial_port"
#define SERIAL_PORT_ARG_SHORT "-s"
#define SERIAL_PORT_ARG_LONG "--serial-port <port>"
#define SERIAL_PORT_ARG_DESC "Serial port to which the Arduino is connected."

int main(int argc, char **argv)
{
	Argx argx;
	argx_init(&argx);

	argx_arg_add(
		HELP_ARG,
		HELP_ARG_SHORT,
		HELP_ARG_LONG,
		HELP_ARG_DESC,
		true,
		&argx
	);
	argx_arg_add(
		UPDATE_INTERVAL_ARG,
		UPDATE_INTERVAL_ARG_SHORT,
		UPDATE_INTERVAL_ARG_LONG,
		UPDATE_INTERVAL_ARG_DESC,
		false,
		&argx
	);
	argx_arg_add(
		INIT_TIMEOUT_ARG,
		INIT_TIMEOUT_ARG_SHORT,
		INIT_TIMEOUT_ARG_LONG,
		INIT_TIMEOUT_ARG_DESC,
		false,
		&argx
	);
	argx_arg_add(
		SERIAL_PORT_ARG,
		SERIAL_PORT_ARG_SHORT,
		SERIAL_PORT_ARG_LONG,
		SERIAL_PORT_ARG_DESC,
		false,
		&argx
	);
	argx_help_msg_gen("memduinod [options]", "Starts the memduino daemon.", &argx);

	argx_args_parse(argv, argc, &argx);

	if (argx_arg_present(HELP_ARG, &argx))
	{
		printf("%s,", argx_help_msg_get(&argx));
		return EXIT_OK;
	}

	unsigned int
		update_interval_ms = 0,
		serial_init_timeout_ms = 0;
	char *serial_port = NULL;

	if (argx_arg_present(SERIAL_PORT_ARG, &argx))
	{
		size_t size;
		argx_arg_val_get_strlen(SERIAL_PORT_ARG, &size, &argx);
		serial_port = malloc(size + 1);
		argx_arg_val_get_str(SERIAL_PORT_ARG, serial_port, &argx);
	}
	else
	{
		printf("%s\n", argx_help_msg_get(&argx));
		printf("No serial port was specified with %s,%s!\n", SERIAL_PORT_ARG_SHORT, SERIAL_PORT_ARG_LONG);
		return EXIT_ARG_MISSING;
	}

	if (argx_arg_present(UPDATE_INTERVAL_ARG, &argx))
		argx_arg_val_get_uint(UPDATE_INTERVAL_ARG, &update_interval_ms, &argx);
	else
		update_interval_ms = DEFAULT_UPDATE_INTERVAL_MS;

	if (argx_arg_present(INIT_TIMEOUT_ARG, &argx))
		argx_arg_val_get_uint(INIT_TIMEOUT_ARG, &serial_init_timeout_ms, &argx);
	else
		serial_init_timeout_ms = DEFAULT_INIT_TIMEOUT_MS;

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
