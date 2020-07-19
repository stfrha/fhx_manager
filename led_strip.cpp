#include "led_strip.h"

LedStrip::LedStrip()
{
   
}

bool LedStrip::initializeLedStrip(void)
{
   pinMode(BLUE_LED, OUTPUT);
   pinMode(RED_LED, OUTPUT);
   pinMode(GREEN_LED, OUTPUT);

   digitalWrite(BLUE_LED, HIGH);
   digitalWrite(RED_LED, HIGH);
   digitalWrite(GREEN_LED, HIGH);
   
   softPwmCreate(BLUE_LED, 0, 100);
   softPwmCreate(RED_LED, 0, 100);
   softPwmCreate(GREEN_LED, 0, 100);
   
   return true;   
}

void LedStrip::setColor(unsigned int color) 
// 0xRRGGBB
{
   setColor((color & 0xff0000) >> 16, (color & 0xff00) >> 8, color & 0xff);
}

void LedStrip::setColor(unsigned char red, unsigned char green, unsigned char blue)
{
   softPwmWrite(RED_LED, 100-red);
   softPwmWrite(GREEN_LED, 100-green);
   softPwmWrite(BLUE_LED, 100-blue);
}

