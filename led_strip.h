
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <iomanip>

#include <wiringPi.h>
#include <softPwm.h>

#include "..\easing-functions\src\easing.h"

#define BLUE_LED 25
#define RED_LED 28
#define GREEN_LED 29

#define SEC_CLOCK_TICK 0.015
#define MS_CLOCK_TICK SEC_CLOCK_TICK * 1000

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
