echo off

echo "Building..."

C:\SysGCC\raspberry\bin\arm-linux-gnueabihf-g++.exe -ggdb main.cpp dali.cpp led_strip.cpp -o fhx_manager -lwiringPi -lpthread

echo "Building complete"


