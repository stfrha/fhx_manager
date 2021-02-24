#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string.h>
#include <time.h>
#include <vector>

#include <wiringPi.h>
#include <wiringSerial.h>

#include "benq.h"

using namespace std;

Benq::Benq()
{
   
}


bool Benq::initializeBenq(void)
{
   m_fd = serialOpen("/dev/ttyUSB0", 9600);
   
   if (m_fd < 0)
   {
      fprintf (stderr, "Unable to open BENQ serial device: %s\n", strerror (errno)) ;
      return false ;
   }
   
   cout << "Let's make sure the receiver buffer is empty, it takes some time..." << endl;
   int l = serialDataAvail(m_fd);
   cout << "There are " << l << " characters in in-buffer." << endl;

   for (int i = 0; i < l; i++)
   {
      serialGetchar(m_fd);
   }
   
   cout << "In-buffer is empty." << endl;

   return true;   
}

int Benq::communicateBenqRequest(const char* cmd, char* reply, int maxReplyLen)
{
   int i = 0;
   
   serialPuts(m_fd, cmd);

   delay(50);

   int l = serialDataAvail(m_fd);

   cout << "There are " << l << " characters in in-buffer." << endl;
  
   for (i = 0; i < min(l, maxReplyLen - 1); i++)
   {
      reply[i] = serialGetchar(m_fd);
   }
   
   reply[i] = 0;
   
   cout << "Response: '" << reply << "'" << endl;
   
   return l;
   
}

bool Benq::isBenqOn(void)
{
   int l = communicateBenqRequest("\r*pow=?#\r", m_reply, 200);
   
   if (l > 0)
   {
      string rep(m_reply);
      
      size_t ansStart = rep.find("*POW=");
      
      if (ansStart != string::npos)
      {
         string ans = rep.substr(ansStart + 5, 3 );
         
         cout << "ans string: '" << ans << "'" << endl;
         
         if (ans == "ON#")
         {
            return true;
         }
      }
      else
      {
         cout << "Did not find search string." << endl;
      }
   }
   
   return false;
}

void Benq::turnOn(void)
{
   if (!isBenqOn())
   {
      communicateBenqRequest("\r*pow=on#\r", m_reply, 200);
   }   
}

void Benq::turnOff(void)
{
   if (isBenqOn())
   {
      cout << "benq is on, now turning it off" << endl;
      
      communicateBenqRequest("\r*pow=off#\r", m_reply, 200);
   }   
}


void Benq::doDummyCommunication(void)
{
   communicateBenqRequest("\r*pow=?#\r", m_reply, 200);
}


