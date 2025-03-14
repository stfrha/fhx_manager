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
  "@MAIN:INP=PHONO\r\n",      //   vinyl,
  "@MAIN:INP=Spotify\r\n",    //   spotify,
  "@MAIN:INP=TUNER\r\n",      //   tuner
  "@MAIN:INP=AV5\r\n",        //   aux, TODO: this is not the correct port!!!
  "@MAIN:INP=USB\r\n",        //   usb
  "@MAIN:INP=AV5\r\n",        //   blu-ray
  "@MAIN:INP=AV4\r\n",        //   raspberryPi
  "@MAIN:INP=AV6\r\n"         //   HDMI
  
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
   
   return ans;
}

void Ynca::turnOn(void)
{
   if (!isYamahaOn())
   {
      char message[] = "@MAIN:PWR=On\r\n";
      m_comms->yamahaComm(message, m_reply, 2000, true);
      
      // Here additional data flows from Yamaha
      // We need to eat that data before we move on
      
      
      
   }
}

void Ynca::turnOff(void)
{
   if (isYamahaOn())
   {
      char message[] = "@MAIN:PWR=Standby\r\n";
      m_comms->yamahaComm(message, m_reply, 2000);
   }
}

void Ynca::startSource(YamahaSourcesEnum source)
{
   if ((source < 0) || (source >= lastSource))
   {
      return;
   }

   turnOn();
   
   // If we set the current source again, there is no reply from 
   // Yamaha, and comm fails. So, only change if we are at
   // a different source
   if (getCurrentSource() != source)
   {
      m_comms->yamahaComm(sourceStrings[source], m_reply, 2000, true);
   }
  
}

void Ynca::volUp(void)
{
   m_comms->yamahaComm("@MAIN:VOL=Up 1 dB\r\n", m_reply, 2000);
}

void Ynca::volDown(void)
{
   m_comms->yamahaComm("@MAIN:VOL=Down 1 dB\r\n", m_reply, 2000);
}

string Ynca::getVolume(void)
{
   char message[] = "@MAIN:VOL=?\r\n";

   if (m_comms->yamahaComm(message, m_reply, 2000) > 0)
   {
      string rep(m_reply);
      
      rep = rep.substr(rep.find("=") + 1);
      rep = rep.substr(0, rep.size() - 2);
      
      cout << "Yamaha volume: " << rep << endl;

      return rep;
   }
   return "-80.0";
}

void Ynca::setVolume(string vol)
{
   // If we set the current volume again, there is no reply from 
   // Yamaha, and comm fails. So, only change if we are at
   // a different volume
   if (getVolume() != vol)
   {
      string msg = "@MAIN:VOL=" + vol + "\r\n";

      cout << "Setting volume: " << vol << endl;

      m_comms->yamahaComm(msg.c_str(), m_reply, 2000);
   }
}

YamahaSourcesEnum Ynca::getCurrentSource(void)
{
   char message[] = "@MAIN:INP=?\r\n";

   if (m_comms->yamahaComm(message, m_reply, 2000) > 0)
   {
      string rep(m_reply);
      
      if (rep == "@MAIN:INP=AV1\r\n")
      {
         return playStation;
      }
      else if (rep == "@MAIN:INP=AV2\r\n")
      {
         return chromecast;
      }
      else if (rep == "@MAIN:INP=AV3\r\n")
      {
         return television;
      }
      else if (rep == "@MAIN:INP=PHONO\r\n")
      {
         return vinyl;
      }
      else if (rep == "@MAIN:INP=Spotify\r\n")
      {
         return spotify;
      }
      else if (rep == "@MAIN:INP=TUNER\r\n")
      {
         return tuner;
      }
      // else if (rep == "@MAIN:INP=AV5\r\n")
      // {
      //    return aux;
      // }
      else if (rep == "@MAIN:INP=USB\r\n")
      {
         return usb;
      }
      else if (rep == "@MAIN:INP=AV5\r\n")
      {
         return bluRay;
      }
      else if (rep == "@MAIN:INP=AV4\r\n")
      {
         return raspberryPi;
      }
      else if (rep == "@MAIN:INP=AV6\r\n")
      {
         return hdmi;
      }
   }
}

