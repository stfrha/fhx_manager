
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <iomanip>

#include <wiringPi.h>
#include <softPwm.h>

#define BLUE_LED 25
#define RED_LED 28
#define GREEN_LED 29

class LedStrip
{
private:

public:   
   LedStrip();
   
   bool initializeLedStrip(void);
   void setColor(unsigned int color); // 0xRRGGBB
   void setColor(unsigned char red, unsigned char green, unsigned char blue);
   
   // Should probably have a destructor that stops all sockets
   

};
