# memduino
Display your computer's RAM usage on a 7 segment 4 digit display connected to an Arduino.<br>

## Setup
- For now memduino needs to be compiled from source.

#### Whether you are on Linux or Windows, you will need to upload the sketch_memduino/sketch_memduino.ino to your Arduino

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

If you want to customize these options you will have to open up /etc/memduino/.argconfig and appends any arguments to<br>
`ARGS=`. <br>
For these new arguments to take effect run:<br>
`systemctl restart memduinod`.<br>
To see a list of all the available arguments run:<br>
`memduinod -h`

### Uninstallation:
Run `sudo make uninstall-daemon` in the build directory.

## Windows:
(broken)
Compile the source however you like and set the meduino executable to run at startup.

## Info
- Only supports RAM values that range from 0 to 99999 (MB) assuming your display has a decimal point.
