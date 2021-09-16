#!/bin/bash

sudo mkdir /etc/memduino || true
if ! [ -f "/etc/memduino/args" ]; then 
    sudo cp $(dirname $BASH_SOURCE)/../res/args /etc/memduino/args
fi
sudo systemctl stop memduinod
sudo cp $(dirname $BASH_SOURCE)/../../build/daemon/memduinod /usr/bin/
sudo cp $(dirname $BASH_SOURCE)/../res/memduinod.service /usr/lib/systemd/system/
sudo chmod 644 /usr/lib/systemd/system/memduinod.service

sudo systemctl enable memduinod
sudo systemctl daemon-reload
sudo systemctl start memduinod
