#!/bin/bash
cd /home/pi/fhx_manager/fhx_manager/build
sleep 5   # let the desktop and network settle
# Videos are shown on the desktop. When started from ssh or a terminal the
# desktop session variables are missing, so point them to the desktop.
export XDG_RUNTIME_DIR="${XDG_RUNTIME_DIR:-/run/user/$(id -u)}"
export WAYLAND_DISPLAY="${WAYLAND_DISPLAY:-wayland-0}"
while true; do
    ./fhx_manager >> /home/pi/fhx_manager/fhx_manager/fhx.log 2>&1
    echo "$(date) fhx_manager exited ($?), restarting" >> /home/pi/fhx_manager/fhx_manager/fhx.log
    sleep 3
done