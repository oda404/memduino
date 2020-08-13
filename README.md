# MemDuino
Display your computer's RAM usage on a 7 segment 4 digit display connected to an Arduino.<br>

## Setup
- Keep in mind that currently MemDuino only works on Linux
- To get this working you will probably need to fiddle with the pin defines
in the arduino sketch and change the deviceName in src/c/main.c
(by default ttyUSB0) to fit your situation.

#### Whether you are on Linux or Windows, you will need to upload the src/arduino/sketch_memDuino.ino to your Arduino

## Linux:
```console
git clone https://github.com/oda404/MemDuino
cd MemDuino
mkdir build && cd build
cmake ..
```
#### If you only want to build the binary, do:
```console
make
```
You can now run memDuino with:
```console
sudo ./memDuino
```
#### If you also want to start memDuino at boot, do:
```console
sudo make install
cd ../src/linux
sudo ./startAtBoot.sh
```

## Info
- Only supports RAM values that range from 0 to 99999 (MB) assuming your display has a DP.