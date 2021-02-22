echo off

echo "Building..."

C:\SysGCC\raspberry\bin\arm-linux-gnueabihf-g++.exe -ggdb main.cpp comms.cpp -o ynca_test -lwiringPi -lpthread

echo "Building complete"


