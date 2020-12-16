#!/usr/bin/bash
#this script should only be executed through make install
sudo cp memDuino /usr/bin/
sudo cp $(dirname $BASH_SOURCE)/memduinod.service /usr/lib/systemd/system/
sudo chmod 644 /usr/lib/systemd/system/memduinod.service
sudo systemctl enable memduinod