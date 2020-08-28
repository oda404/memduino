// Copyright (c) Olaru Alexandru <xdxalexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#include"memDuino.h"

int main(void)
{
	memduino memduino;
	memduino.info.update_interval_ms = 1000;
	memduino.info.device_name = "ttyUSB0";
	memduino.info.init_timeout_ms = 10000;

	start_memduino(&memduino);
/*
#define DIV 1048576

	serialInit("COM3");

	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);


	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);

	int usedMem,
		usedMemLen;

	while (1)
	{
		GlobalMemoryStatusEx(&statex);

		usedMem = (float)statex.dwMemoryLoad / 100.0f * (float)(statex.ullTotalPhys / DIV);

		usedMemLen = getIntLen(usedMem);
		char *usedMemStr = malloc(usedMemLen + 3);
		createPacket(usedMem, usedMemLen, usedMemStr);

		writeToSerial(usedMemStr);
		
		free(usedMemStr);

		Sleep(1000);
	}

	serialClose();

#endif

	return 0;*/
}
