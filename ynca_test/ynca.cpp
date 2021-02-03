#include <iostream>
#include <string.h>

#include "ynca.h"

using namespace std;

// Maintain this order:
// playStation,
// chromecast,
// television,
// vinyl,
// spotify,
// tuner,
// aux,
// usb,
// bluRay,
// raspberryPi,


const char* const Ynca::sourceStrings[] = {
  "@MAIN:INP=AV1\r\n",        //   playStation
  "@MAIN:INP=AV2\r\n",        //   chromecast,
  "@MAIN:INP=AV3\r\n",        //   television,
  "@MAIN:INP=Phono\r\n",      //   vinyl,
  "@MAIN:INP=Spotify\r\n",    //   spotify,
  "@MAIN:INP=Tuner\r\n",      //   tuner
  "@MAIN:INP=AV5\r\n",        //   aux
  "@MAIN:INP=USB\r\n",        //   usb
  "@MAIN:INP=AV5\r\n",        //   blu-ray
  "@MAIN:INP=AV4\r\n"         //   raspberryPi
};

Ynca::Ynca(Comms* comms) :
   m_comms(comms)
{   
}


void Ynca::initialize(void)
{
}

bool Ynca::isYamahaOn(void)
{
   char message[] = "@MAIN:PWR=?\r\n";
   char gotValidAnswer = false;
   char ans = false;

   if (m_comms->yamahaComm(message, m_reply, 2000) > 0)
   {
      string rep(m_reply);
      
      if (rep == "@MAIN:PWR=Standby\r\n")
      {
         ans = false;
         gotValidAnswer = true;
      }
      else if (rep == "@MAIN:PWR=On\r\n")
      {
         ans = true;
         gotValidAnswer = true;
      }
   }



//   int tries = 0;
//   
//   while (!gotValidAnswer && (tries < 4))
//   {
//      if (m_comms->yamahaComm(message, m_reply, 2000) > 0)
//      {
//         string rep(m_reply);
//         
//         if (rep == "@MAIN:PWR=Standby\r\n")
//         {
//            ans = false;
//            gotValidAnswer = true;
//         }
//         else if (rep == "@MAIN:PWR=On\r\n")
//         {
//            ans = true;
//            gotValidAnswer = true;
//         }
//      }
//      tries++;
//   }
//   
//   if (tries >= 5)
//   {
//      cout << "Did not get a response from yamaha, even after " << tries << " tries." << endl;
//   }
   
   return ans;
}

void Ynca::turnOn(void)
{
   if (!isYamahaOn())
   {
      char message[] = "@MAIN:PWR=On\r\n";
      m_comms->yamahaComm(message, m_reply, 2000);
   }
   
   
//   int tries = 0;
//   while (!isYamahaOn() && (tries < 4))
//   {
//      char message[] = "@MAIN:PWR=On\r\n";
//
//      m_comms->yamahaComm(message, m_reply, 200);
//   }
//
//   if (tries >= 5)
//   {
//      cout << "Was not able to turn on yamaha, even after " << tries << " tries." << endl;
//   }
}

void Ynca::turnOff(void)
{
   if (isYamahaOn())
   {
      char message[] = "@MAIN:PWR=Standby\r\n";
      m_comms->yamahaComm(message, m_reply, 2000);
   }

//   int tries = 0;
//
//   while (isYamahaOn() && (tries < 4))
//   {
//      char message[] = "@MAIN:PWR=Standby\r\n";
//
//      m_comms->yamahaComm(message, m_reply, 200);
//   }
//
//   if (tries >= 5)
//   {
//      cout << "Was not able to turn off yamaha, even after " << tries << " tries." << endl;
//   }
}

void Ynca::startSource(YamahaSourcesEnum source)
{
   int tries = 0;

   if ((source < 0) || (source >= lastSource))
   {
      return;
   }

   turnOn();
   m_comms->yamahaComm(sourceStrings[source], m_reply, 2000);
   
//   do
//   {
//      m_comms->yamahaComm(sourceStrings[source], m_reply, 200);
//   } while ((string(m_reply) != sourceStrings[source]) && (tries < 5));
//      
//   if (tries >= 5)
//   {
//      cout << "Was not able to change source, even after " << tries << " tries." << endl;
//   }
}

void Ynca::volUp(void)
{
   m_comms->yamahaComm("@MAIN:VOL=Up 1 dB\r\n", m_reply, 2000);
}

void Ynca::volDown(void)
{
   m_comms->yamahaComm("@MAIN:VOL=Down 1 dB\r\n", m_reply, 2000);
}


