# memduino
Display your computer's RAM usage on a 7 segment 4 digit display connected to an Arduino.<br>

## Installation
For now memduino needs to be compiled from source.

### Arduino:
* Configure the first 12 macros in sketch_memduino/sketch_memduino.ino to fit your particular setup. 
* Upload sketch_memduino/sketch_memduino.ino to your Arduino.

### Linux:
```console
git clone https://github.com/oda404/memduino
cd memduino
cmake -B build .
sudo make -C build
sudo make -C build install-all
```

### Any other platform:
Feel free to port it.

## Post-installation

### Options:
To see a list of all the available options run:
```console
memduinod -h
```

### Running:
You can either run `memduinod` in a terminal, passing it any options you want. Example:
```console
sudo memduinod --serial-port /dev/ttyUSB0 --update-interval 500
```
or you can run `memduinod` as a systemd service by first appending any options you want to the <b>*MEMDUINO_ARGS*</b> variable in `/etc/memduino/args`,
and starting it with:
```console
sudo systemctl start memduinod
```
If you also want `memduinod` to start at boot run:
```console
sudo systemctl enable memduinod
```

## Uninstallation
```console
cd <repo-location>
sudo make -C build uninstall-all
``` 
## License
This code is licensed under the MIT license.