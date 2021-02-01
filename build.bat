echo off

echo "Building..."

C:\SysGCC\raspberry\bin\arm-linux-gnueabihf-g++.exe -ggdb main.cpp comms.cpp controller.cpp dali.cpp led_strip.cpp easing.cpp ynca.cpp benq.cpp -o fhx_manager -lwiringPi -lpthread

echo "Building complete"


