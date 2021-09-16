// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#ifndef MEMDUINO_H
#define MEMDUINO_H

typedef struct
S_MemduinoConfig
{
    unsigned int update_interval_ms;
    unsigned int serial_init_timeout_ms;
    char *serial_port;
} MemduinoConfig;

int start_memduino(
    const MemduinoConfig *config
);

#endif // MEMDUINO_H
