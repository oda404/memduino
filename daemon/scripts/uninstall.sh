#!/bin/bash

sudo systemctl disable --now memduinod
sudo rm -f /usr/lib/systemd/system/memduinod.service /usr/bin/memduinod
sudo rm -rf /etc/memduino
