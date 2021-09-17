#!/bin/bash

mkdir /etc/memduino 2> /dev/null || true
if ! [ -f "/etc/memduino/args" ]; then 
    echo "MEMDUINO_ARGS=" > /etc/memduino/args
fi

systemctl disable --now memduinod
cp $(dirname $BASH_SOURCE)/../../build/daemon/memduinod /usr/bin/
cp $(dirname $BASH_SOURCE)/../res/memduinod.service /usr/lib/systemd/system/
chmod 644 /usr/lib/systemd/system/memduinod.service

systemctl daemon-reload
