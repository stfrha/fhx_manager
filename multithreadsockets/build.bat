echo off

echo "Building..."

C:\SysGCC\raspberry\bin\arm-linux-gnueabihf-g++.exe -ggdb main.cpp  -o multithreadsockets -lwiringPi -lpthread
REM C:\SysGCC\raspberry\bin\arm-linux-gnueabihf-g++.exe -ggdb test1.cpp -o test1  

echo "Building complete"


