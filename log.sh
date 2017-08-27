#!/bin/bash

if [ ! $EUID == "0" ]; then
   echo Must run as root
   exit
fi
	
chmod 0666 /dev/ttyUSB0

while [ true ]; do
    stty -F /dev/ttyUSB0 -hupcl
    /home/peter/bin/ttylog -b 115200 -d /dev/ttyUSB0 -f
    # ttylog -b 115200 -d /dev/ttyACM0 -f |\
	# gawk '{ print strftime("%c  [%s] "), $0; fflush(); }'
	# awk '{ print $8; fflush(); }'
    sleep 1
done
