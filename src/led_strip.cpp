// For threads
#include <pthread.h>

#include "led_strip.h"

#include <wiringPiSPI.h>

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
   // This is the old setup, not use now for tlc59711
   // pinMode(BLUE_LED, OUTPUT);
   // pinMode(RED_LED, OUTPUT);
   // pinMode(GREEN_LED, OUTPUT);

   // digitalWrite(BLUE_LED, HIGH);
   // digitalWrite(RED_LED, HIGH);
   // digitalWrite(GREEN_LED, HIGH);

   // softPwmCreate(RED_LED, 0, 100);
   // softPwmCreate(GREEN_LED, 0, 100);
   // softPwmCreate(BLUE_LED, 0, 100);

   m_tlc.begin();

   // unsigned char buf[28];

   // unsigned char seed = 45;

   // for (int i = 0; i < 28; i++)
   // {
   //    buf[i] = i + seed;
   // }

   // cout << "Writing with seed: " << (int)seed << endl;

   // wiringPiSPIDataRW(0, buf, 28);

   // cout << "TLC59711 buffer dump:" << endl;

   // for (int i = 0; i < 28; i++)
   // {
   //    cout << i << ", " << (int)buf[i] << endl;
   // }

   // seed = 78;

   // cout << "Writing with seed: " << (int)seed << endl;

   // for (int i = 0; i < 28; i++)
   // {
   //    buf[i] = i + seed;
   // }

   // wiringPiSPIDataRW(0, buf, 28);

   // cout << "TLC59711 buffer dump:" << endl;

   // for (int i = 0; i < 28; i++)
   // {
   //    cout << i << ", " << (int)buf[i] << endl;
   // }


   setColor(0xfff, 0xfff, 0xfff);

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
   // We are loosing some dynamics here, but it is ok.
   setColor(((color & 0xff0000) >> 16) * 16, ((color & 0xff00) >> 8) * 16, (color & 0xff) * 16);
}

void LedStrip::setColor(int red, int green, int blue)
{
   m_red = red;
   m_green = green;
   m_blue = blue;

   m_tlc.setLED(0, red, green, blue);
   m_tlc.setLED(1, red, green, blue);
   m_tlc.setLED(2, red, green, blue);
   m_tlc.setLED(3, red, green, blue);
   m_tlc.write();

   // softPwmWrite(RED_LED, 100-m_red);
   // softPwmWrite(GREEN_LED, 100-m_green);
   // softPwmWrite(BLUE_LED, 100-m_blue);
   // cout << "Writing color, r: " << 100 - m_red << ", g: " << 100 - m_green << ", b: " << 100 - m_blue << endl;
   
}

void LedStrip::fadeToColor(unsigned int color, double duration, easing_functions easingFunction) // 0xRRGGBB
{
   fadeToColor(((color & 0xff0000) >> 16) * 16, ((color & 0xff00) >> 8) * 16, (color & 0xff) * 16, duration, easingFunction);
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


// Code below is found by https://github.com/barrymcandrews/tlc59711/blob/master/src/tlc59711.cpp
// but it is slightly adapted by me.

TLC59711::TLC59711(uint8_t n) {
  numdrivers = n;

  BCr = BCg = BCb = 0x7F; // default 100% brigthness
}

/*!
 *  @brief  Writes PWM buffer to board
 */
void TLC59711::write() {

  unsigned char command_buffer[28] = {0};
  uint32_t command;

  // Magic word for write
  command = 0x25;

  command <<= 5;
  // OUTTMG = 1, EXTGCK = 0, TMGRST = 1, DSPRPT = 1, BLANK = 0 -> 0x16
  command |= 0x16;

  command <<= 7;
  command |= BCr;

  command <<= 7;
  command |= BCg;

  command <<= 7;
  command |= BCb;

  command_buffer[0] = (char) ((command >> 24) & 0xff);
  command_buffer[1] = (char) ((command >> 16) & 0xff);
  command_buffer[2] = (char) ((command >> 8) & 0xff);
  command_buffer[3] = (char) ((command >> 0) & 0xff);

  uint8_t pwmbuffer_reversed[24];
  for (int i = 0; i < 24; i++) {
    pwmbuffer_reversed[i] = ((uint8_t *)pwmbuffer)[23 - i];
  }

  memcpy(command_buffer + 4, pwmbuffer_reversed, 24);
  wiringPiSPIDataRW(0, command_buffer, 28);

//  for (int i = 4; i < 6; i++) {
//    std::cout << i << "        ";
//  }
//  std::cout << std::endl;
//
//  for (int i = 4; i < 6; i++) {
//    std::bitset<8> x(command_buffer[i]);
//    std::cout << x << " ";
//  }
//  std::cout << std::endl;
}

/*!
 *  @brief  Set PWM value on selected channel
 *  @param  chan
 *          one from 12 channel (per driver) so there is 12 * number of drivers
 *  @param  pwm
 *          pwm value
 */
void TLC59711::setPWM(uint8_t chan, uint16_t pwm) {
  if (chan > 12 * numdrivers)
    return;
  pwmbuffer[chan] = pwm;
}

/*!
 *  @brief  Set RGB value for selected LED
 *  @param  lednum
 *          selected LED number that for which value will be set
 *  @param  r
 *          red value
 *  @param g
 *          green value
 *  @param b
 *          blue value
 */
void TLC59711::setLED(uint8_t lednum, uint16_t r, uint16_t g,
                               uint16_t b) {
  setPWM(lednum * 3, r);
  setPWM(lednum * 3 + 1, g);
  setPWM(lednum * 3 + 2, b);
}

/*!
 *  @brief  Set the brightness of LED channels to same value
 *  @param  BC
 *          Brightness Control value
 */
void TLC59711::simpleSetBrightness(uint8_t BC) {
  if (BC > 127) {
    BC = 127; // maximum possible value since BC can only be 7 bit
  } else if (BC < 0) {
    BC = 0;
  }

  BCr = BCg = BCb = BC;
}

/*!
 *  @brief  Set the brightness of LED channels to specific value
 *  @param  bcr
 *          Brightness Control Red value
 *  @param  bcg
 *          Brightness Control Green value
 *  @param  bcb
 *          Brightness Control Blue value
 */
void TLC59711::setBrightness(uint8_t bcr, uint8_t bcg, uint8_t bcb) {
  if (bcr > 127) {
    bcr = 127; // maximum possible value since BC can only be 7 bit
  } else if (bcr < 0) {
    bcr = 0;
  }

  BCr = bcr;

  if (bcg > 127) {
    bcg = 127; // maximum possible value since BC can only be 7 bit
  } else if (bcg < 0) {
    bcg = 0;
  }

  BCg = bcg;

  if (bcb > 127) {
    bcb = 127; // maximum possible value since BC can only be 7 bit
  } else if (bcb < 0) {
    bcb = 0;
  }

  BCb = bcb;
}

/*!
 *  @brief  Begins SPI connection if there is not empty PWM buffer
 *  @return If successful returns true, otherwise false
 */
bool TLC59711::begin() {

   int spi_result = wiringPiSPISetup(0, 5000000);
   cout << "Initialise SPI returned: " << spi_result << endl;
 
  return true;
}

void TLC59711::end() {
   // TODO: IS there any wiringPI end?
}