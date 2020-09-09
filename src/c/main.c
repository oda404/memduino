// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#include"memDuino.h"

int main(void)
{
	MemDuino memduino;
#if defined(_WIN32)
	memduino.info.device_name = "COM3";
#elif defined(__linux__)
	memduino.info.device_name = "ttyUSB0";
#endif // _WIN32
	memduino.info.update_interval_ms = 1000;
	memduino.info.init_timeout_ms = 10000;

	start_memduino(&memduino);
	
	return 0;
}
