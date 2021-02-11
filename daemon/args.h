// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#ifndef MEMDUINO_ARGS_H
#define MEMDUINO_ARGS_H

#define UI unsigned int
#define ARG_OK 0
#define ARG_NOT_FOUND 1
#define ARG_BAD 2

UI check_for_help_arg(int argc, char **argv);
UI get_update_interval_arg(int argc, char **argv, UI *out);
UI get_init_timeout_arg(int argc, char **argv, UI *out);
UI get_init_try_sleep_arg(int argc, char **argv, UI *out);
UI get_serial_port_arg(int argc, char **argv, char *out);

#endif // MEMDUINO_ARGS_H