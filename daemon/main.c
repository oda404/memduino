// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#include"memDuino.h"

#define DEFAULT_UPDATE_INTERVAL_MS 1000
#define DEFAULT_INIT_TIMEOUT_MS 10000

int main(void)
{
	return start_memduino(
		DEFAULT_UPDATE_INTERVAL_MS, 
		DEFAULT_INIT_TIMEOUT_MS
	);
}
