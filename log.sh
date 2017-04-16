#!/bin/bash

# SKAL KOERE SOM ROOT!

chmod 0666 /dev/ttyACM0

while [ true ]; do
    stty -F /dev/ttyACM0 -hupcl
    /home/peter/bin/ttylog -b 115200 -d /dev/ttyACM0 -f
    # ttylog -b 115200 -d /dev/ttyACM0 -f |\
	# gawk '{ print strftime("%c  [%s] "), $0; fflush(); }'
	# awk '{ print $8; fflush(); }'
    sleep 1
done
