// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#ifndef MEMDUINO_SERIAL_H
#define MEMDUINO_SERIAL_H

#include"memDuino.h"

int try_serial_init(const char *device_name, int *device_fd);
void write_to_serial(const int *device_fd, const char *data);
void serial_close(const int *device_fd);

#endif // MEMDUINO_SERIAL_H
