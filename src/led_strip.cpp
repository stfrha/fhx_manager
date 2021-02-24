// For threads
#include <pthread.h>

#include "led_strip.h"


using namespace std;

ColorTween::ColorTween() :
   m_tweenActive(false),
   m_startValue(0.0),
   m_endValue(1.0),
   m_duration(1.0),
   m_startTime(0.0),
   m_easingFunction(EaseLinear)
{

}

void ColorTween::startTween(double start, double end, double startTime, double duration, easing_functions easingFunction)
{
   m_startValue = start;
   m_endValue = end;
   m_duration = duration;
   m_startTime = startTime;
   m_easingFunction = easingFunction;
   m_tweenActive = true;

   // cout << "Start value: " << start << ", end value: " << end << endl;
}

bool ColorTween::isActive(void)
{
   return m_tweenActive;
}

double ColorTween::getEasingFactor(double time)  // time is 0.0->1.0
{
   auto easingFunction = getEasingFunction(m_easingFunction);


   double f = easingFunction(time);

   //cout << "Easing Factor: " << f  << ", at time: " << time << endl;

   return f;
}

double ColorTween::getEasingValue(double time)
{
   if (time > m_startTime + m_duration)
   {
      m_tweenActive = false;
      return m_startValue + (m_endValue - m_startValue) * getEasingFactor(1.0);
   }
   double v = m_startValue + (m_endValue - m_startValue) * getEasingFactor((time - m_startTime) / m_duration);

   // cout << "Easing Value: " << v << endl;

   return v;
}

LedStrip::LedStrip()
{
   m_time = 0;
}

bool LedStrip::initializeLedStrip(void)
{
   pinMode(BLUE_LED, OUTPUT);
   pinMode(RED_LED, OUTPUT);
   pinMode(GREEN_LED, OUTPUT);

   digitalWrite(BLUE_LED, HIGH);
   digitalWrite(RED_LED, HIGH);
   digitalWrite(GREEN_LED, HIGH);

   softPwmCreate(RED_LED, 0, 100);
   softPwmCreate(GREEN_LED, 0, 100);
   softPwmCreate(BLUE_LED, 0, 100);

   setColor(100, 100, 100);

   pthread_t threadId;

   int result = pthread_create(&threadId, NULL, stepThread, (void*)this);
   if (result)
   {
      cout << "Step Thread could not be created, " << result << endl;
      exit(1);
   }

   return true;   
}

void LedStrip::setColor(unsigned int color) 
// 0xRRGGBB
{
   setColor((color & 0xff0000) >> 16, (color & 0xff00) >> 8, color & 0xff);
}

void LedStrip::setColor(int red, int green, int blue)
{
   m_red = red;
   m_green = green;
   m_blue = blue;

   softPwmWrite(RED_LED, 100-m_red);
   softPwmWrite(GREEN_LED, 100-m_green);
   softPwmWrite(BLUE_LED, 100-m_blue);
   // cout << "Writing color, r: " << 100 - m_red << ", g: " << 100 - m_green << ", b: " << 100 - m_blue << endl;
   
}

void LedStrip::fadeToColor(unsigned int color, double duration, easing_functions easingFunction) // 0xRRGGBB
{
   fadeToColor((color & 0xff0000) >> 16, (color & 0xff00) >> 8, color & 0xff, duration, easingFunction);
}

void LedStrip::fadeToColor(int red, int green, int blue, double duration, easing_functions easingFunction)
{
   m_redTween.startTween(m_red, red, m_time, duration, easingFunction);
   m_greenTween.startTween(m_green, green, m_time, duration, easingFunction);
   m_blueTween.startTween(m_blue, blue, m_time, duration, easingFunction);
}

bool LedStrip::isFadeComplete(void)
{
   if (m_redTween.isActive())
   {
      return false;
   }

   if (m_greenTween.isActive())
   {
      return false;
   }

   if (m_blueTween.isActive())
   {
      return false;
   }

   return true;
}

void LedStrip::update(void)
{
   m_time += SEC_CLOCK_TICK;

   double red = m_red;
   double green = m_green;
   double blue = m_blue;

   if (m_redTween.isActive())
   {
      red = m_redTween.getEasingValue(m_time);
   }

   if (m_greenTween.isActive())
   {
      green = m_greenTween.getEasingValue(m_time);
   }

   if (m_blueTween.isActive())
   {
      blue = m_blueTween.getEasingValue(m_time);
   }

   // cout << "Setting colors, red: " << (int)red << ", green: " << (int)green << ", blue: " << (int)blue << endl;

   setColor((int)red, (int)green, (int)blue);

}

// Clock tick thread
void* LedStrip::stepThread(void* dummy)
{
   LedStrip* ls = (LedStrip*)dummy;

   while (true)
   {
      delay(MS_CLOCK_TICK);
      ls->update();
   }
}

