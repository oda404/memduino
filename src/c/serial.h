// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#ifndef SERIAL_W_H
#define SERIAL_W_H

#include"memDuino.h"

int serial_init(memduino *md);
void write_to_serial(memduino *md, const char *data);
void serial_close(memduino *md);

#endif // SERIAL_W_H
