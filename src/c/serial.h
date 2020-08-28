// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#ifndef SERIAL_W_H
#define SERIAL_W_H

#include"memDuino.h"

int serial_init(memduino *md);
void write_to_serial(int fd, const char *data);
void serial_close(int fd);

#endif // SERIAL_W_H
