// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#ifndef MEMDUINO_SERIAL_H
#define MEMDUINO_SERIAL_H

#include"memDuino.h"

int serial_init(MemDuino *memduino);
void write_to_serial(const MemDuino *memduino, const char *data);
void serial_close(const MemDuino *memduino);

#endif // MEMDUINO_SERIAL_H
