#!/bin/bash

# chmod 0666 /dev/ttyACM0

# ttylog -b 115200 -d /dev/ttyACM0 -f | gawk '{ print strftime("%c  [%s] "), $0; fflush(); }'

while [ true ]; do
    stty -F /dev/ttyACM0 -hupcl
    /home/peter/bin/ttylog -b 115200 -d /dev/ttyACM0 -f
    sleep 1
done
