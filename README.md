## MemDuino
Display your computer's RAM usage on a 7 segment 4 digit display.<br>

## Setup
- Keep in mind that currently MemDuino only works on Linux
- To get this working you will probably need to fiddle with the pin defines
in the arduino sketch and change the device name used in the serialInit() function in main.c
(by default ttyUSB0) to fit your situation.
- To make the C program cd to the MemDuino directory and simply type make. A new file named memDuino.out should pop up.
- MemDuino works through serial communication, so after the sketch is uploaded to the Arduino,
and the memDuino.out binary is run, you should see your RAM usage displayed.
- If you run memDuino.out and nothing happens you could try running it as root.

## Info
- Only supports RAM values that range from 0 to 99999 (MB) assuming your display has a DP.
