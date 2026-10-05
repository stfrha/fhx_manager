#!/bin/bash
cd /home/pi/fhx_manager/fhx_manager/build
sleep 5   # let the desktop and network settle
while true; do
    ./fhx_manager >> /home/pi/fhx_manager/fhx_manager/fhx.log 2>&1
    echo "$(date) fhx_manager exited ($?), restarting" >> /home/fredrik/fhx_manager/fhx.log
    sleep 3
done