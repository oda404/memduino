// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#ifndef MEMDUINO_SERIAL_H
#define MEMDUINO_SERIAL_H

#include<stddef.h>

/**
 * Initiates a serial connection to the given device.
 * @param device Device path
 * @returns The device file descriptor if successful, a negative value otherwise.
*/
int serial_init(const char *device);
/**
 * Writes a null terminated string to the given device.
 * @param device_fd The device file descriptor.
 * @param data NULL terminated string.
 * @returns Number of bytes written.
*/
size_t serial_write(int device_fd, const char *data);
/**
 * Writes 'n' bytes from 'data' to the given device.
 * @param device_fd The device file descriptor.
 * @param data Buffer of data.
 * @param n Number of bytes to write.
 * @returns Number of bytes written.
*/
size_t serial_nwrite(int device_fd, const char *data, size_t n);
/**
 * Close the serial connection for the given device.
 * @param device_fd The device file descriptor
*/
void serial_close(int device_fd);

#endif // MEMDUINO_SERIAL_H
