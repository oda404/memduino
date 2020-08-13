// Copyright (c) Olaru Alexandru <olarualexandru404@gmail.com>
// Licensed under the MIT license found in the LICENSE file in the root of this repository.

#include"memDuino.h"

int main(void)
{
	MemDuino memDuino;
	memDuino.updateIntervalMs = 1000;
	memDuino.deviceName = "ttyUSB0";

	startMemDuino(&memDuino);
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
