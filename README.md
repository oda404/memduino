# memduino
Display your computer's RAM usage on a 7 segment 4 digit display connected to an Arduino.<br>

## Setup
- For now memduino needs to be compiled from source.

#### Whether you are on Linux or Windows, you will need to upload the skerch_memduino/sketch_memduino.ino to your Arduino

## Linux:
```console
git clone https://github.com/oda404/memduino
cd memduino
mkdir build && cd build
cmake ..
make
sudo make install-daemon
```

This will install memduinod as a systemd service and will run with the default options:
- Update interval: 1000ms
- Init timeout: 10000ms
- Init try sleep time: 1500ms (sleep time between serial init attempts)
- Serial port: /dev/ttyUSB0

If you want to customize these options you will have to run memduinod from the terminal (at least for now). For that, first stop the systemd service:<br>
`# systemctl stop memduinod`<br>
`# systemctl disable memduinod`<br>
And then you can run:<br>
`$ memduinod -h`<br>
To see all the available options.
### Uninstallation:
Run `sudo make uninstall-daemon` in the build directory.

## Windows:
(broken)
Compile the source however you like and set the meduino executable to run at startup.

## Info
- Only supports RAM values that range from 0 to 99999 (MB) assuming your display has a decimal point.
