#ifndef LEDSSTRIP_H
#define LEDSSTRIP_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <iomanip>

#include <wiringPi.h>
#include <softPwm.h>

//#include "..\easing-functions\src\easing.h"
#include "easing.h"

#define BLUE_LED 25
#define RED_LED 29
#define GREEN_LED 28

#define SEC_CLOCK_TICK 0.010
#define MS_CLOCK_TICK SEC_CLOCK_TICK * 1000

// Code below is found by https://github.com/barrymcandrews/tlc59711/blob/master/include/tlc59711.h
// but it is slightly adapted by me.

/*!
 *  @brief  Class that stores state and functions for interacting with
 *          TLC59711 Senor
 */
class TLC59711 {
   public:
      explicit TLC59711(uint8_t n);
   
      bool begin();
      void end();
   
      void setPWM(uint8_t chan, uint16_t pwm);
      void setLED(uint8_t lednum, uint16_t r, uint16_t g, uint16_t b);
      void write();
      void setBrightness(uint8_t bcr, uint8_t bcg, uint8_t bcb);
      void simpleSetBrightness(uint8_t BC);
   
   private:
      uint16_t pwmbuffer[12] = {0};
   
      uint8_t BCr, BCg, BCb;
      int8_t numdrivers;
      bool initialized = false;
   };
   
class ColorTween
{
private:
   bool m_tweenActive;
   double m_startValue;
   double m_endValue;
   double m_duration;
   double m_startTime;
   easing_functions m_easingFunction;

   double getEasingFactor(double time);  // time is 0.0->1.0

public:
   ColorTween();
   void startTween(double start, double end, double startTime, double duration, easing_functions easingFunction = EaseLinear);
   bool isActive(void);
   double getEasingValue(double time);
};


class LedStrip
{
private:

   TLC59711 m_tlc = TLC59711(1);

   int m_red;
   int m_green;
   int m_blue;
   ColorTween m_redTween;
   ColorTween m_greenTween;
   ColorTween m_blueTween;
   double m_time; // In seconds

   static void* stepThread(void* threadId);

public:   
   LedStrip();
   
   bool initializeLedStrip(void);
   void setColor(unsigned int color); // 0xRRGGBB
   void setColor(int red, int green, int blue);
   void fadeToColor(unsigned int color, double duration, easing_functions easingFunction = EaseLinear); // 0xRRGGBB
   void fadeToColor(int red, int green, int blue, double duration, easing_functions easingFunction = EaseLinear);
   bool isFadeComplete(void);
   void update(void);

   // Should probably have a destructor that stops all sockets
   

};




#endif

