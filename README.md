# memDuino
Display your computer's RAM usage on a 7 segment 4 digit display connected to an Arduino.<br>

## Setup
- For now memDuino needs to be compiled from source.
- To get this working you will probably need to fiddle with the pin defines
in the arduino sketch and change the deviceName in src/c/main.c
(by default ttyUSB0) to fit your situation.

#### Whether you are on Linux or Windows, you will need to upload the src/arduino/sketch_memDuino.ino to your Arduino

## Linux:
```console
git clone https://github.com/oda404/memDuino
cd MemDuino
mkdir build && cd build
cmake ..
make
sudo make install
```
### Uninstallation:
Run `src/linux/uninstall.sh`

## Windows:
Compile the source however you like and set the memDuino executeable to run at startup.

## Info
- Only supports RAM values that range from 0 to 99999 (MB) assuming your display has a decimal point.
