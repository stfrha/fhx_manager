
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <iomanip>

#include "led_strip.h"
#include "Dali.h"

using namespace std;

// The message queue
pthread_mutex_t msgQueuMutex = PTHREAD_MUTEX_INITIALIZER;
vector<string> g_messageQueue;

// Conditional variable to indicate message in queue
pthread_cond_t g_cv;
pthread_mutex_t g_cvLock;

/* Messages are:

STEP  - Execute step of measurements and pump control
AUTO  - Go to automatic control
MANL  - Go to manual control
S_ON  - Force Solar pump on, ignored in AUTO
SOFF  - Force Solar pump off, ignored in AUTO
F_ON  - Force Filter pump on, ignored in AUTO
FOFF  - Force Filter pump off, ignored in AUTO

*/



int main(int argc, char *argv[])
{
   LedStrip ledStrip;
   Dali dali;


   cout << "Welcome to FHX-manager!" << endl;

   
   if (wiringPiSetup () == -1)
   {
      fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
      return 1 ;
   }

   if (!ledStrip.initializeLedStrip())
   {
      cout << "Error initializing Led strip." << endl;
      return 1;
   }

   if (!dali.initializeDali())
   {
      cout << "Error initializing Dali." << endl;
      return 1;
   }

   //while (true)
   //{
   //   cout << "Fade all on" << endl;
   //   ledStrip.fadeToColor(0x64, 0x64, 0x64, 4.5, EaseInQuad);
   //   while (!ledStrip.isFadeComplete())
   //   {
   //      delay(500);
   //   }

   //   cout << "Fade to red" << endl;
   //   ledStrip.fadeToColor(0x64, 0x0, 0x0, 4.5, EaseInQuad);
   //   while (!ledStrip.isFadeComplete())
   //   {
   //      delay(500);
   //   }

   //   cout << "Fade to green" << endl;
   //   ledStrip.fadeToColor(0x0, 0x64, 0x0, 4.5, EaseInQuad);
   //   while (!ledStrip.isFadeComplete())
   //   {
   //      delay(500);
   //   }

   //   cout << "Fade to blue" << endl;
   //   ledStrip.fadeToColor(0x0, 0x0, 0x64, 4.5, EaseInQuad);
   //   while (!ledStrip.isFadeComplete())
   //   {
   //      delay(500);
   //   }

   //   cout << "Fade to black" << endl;
   //   ledStrip.fadeToColor(0x0, 0x0, 0x0, 4.5, EaseInQuad);
   //   while (!ledStrip.isFadeComplete())
   //   {
   //      delay(500);
   //   }
   //}



   bool isOn = dali.isLightsOn();

   while (true)
   {
      if (dali.isLightsOn())
      {
         if (!isOn)
         {
            cout << "Led stripe on at random color" << endl;

            int colorCode = rand() % 7;
            switch (colorCode)
            {
            case 0:
               ledStrip.fadeToColor(0x64, 0x64, 0x64, 4.5, EaseInQuad);
               break;
            case 1:
               ledStrip.fadeToColor(0x64, 0x0, 0x0, 4.5, EaseInQuad);
               break;
            case 2:
               ledStrip.fadeToColor(0x0, 0x64, 0x00, 4.5, EaseInQuad);
               break;
            case 3:
               ledStrip.fadeToColor(0x0, 0x0, 0x64, 4.5, EaseInQuad);
               break;
            case 4:
               ledStrip.fadeToColor(0x64, 0x64, 0x0, 4.5, EaseInQuad);
               break;
            case 5:
               ledStrip.fadeToColor(0x0, 0x64, 0x64, 4.5, EaseInQuad);
               break;
            case 6:
               ledStrip.fadeToColor(0x64, 0x0, 0x64, 4.5, EaseInQuad);
               break;
            }
         }
         isOn = true;
      }
      else
      {
         if (isOn)
         {
            cout << "Led stripe off" << endl;
            ledStrip.fadeToColor(0, 4.5, EaseInQuad);
         }
         isOn = false;
      }
      delay(250);

      //dali.setLightPower(0, 0);
      //dali.setLightPower(1, 0);
      //for (int i = 0; i < 100 ; i++)
      //{
      //   ledStrip.setColor(0, 0, i);
      //   delay(15);
      //}
      //dali.setLightPower(0, 0xfe);
      //dali.setLightPower(1, 0xfe);
      //for (int i = 100; i >= 0 ; i--)
      //{
      //   ledStrip.setColor(0, 0, i);
      //   delay(15);
      //}
   }

   dali.terminate();
   
   return 0;
}
