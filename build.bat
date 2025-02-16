
echo off

echo "Building..."

C:\SysGCC\raspberry\bin\arm-linux-gnueabihf-g++.exe -ggdb main.cpp comms.cpp controller.cpp dali.cpp led_strip.cpp easing.cpp ir.cpp ynca.cpp benq.cpp -o fhx_manager -lwiringPi -lpthread -llirc_client

echo "Building complete"


