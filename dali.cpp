#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string.h>
#include <time.h>
#include <vector>

#include <wiringPi.h>
#include <wiringSerial.h>

#include "dali.h"

using namespace std;

Dali::Dali()
{
   
}


bool Dali::initializeDali(void)
{
   char respBuf[10];

   m_fd = serialOpen("/dev/serial0", 19200);
   
   if (m_fd < 0)
   {
      fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
      return false ;
   }
   
   // Test serial and DALI interface
   // write version request
   cout << "Sending version request..." << endl;
   communicateCommand("v\n", 7, respBuf);

   cout << "Sending power request..." << endl;
   communicateCommand("p\n", 8, respBuf);

   cout << "Sending power status request..." << endl;
   communicateCommand("d\n", 3, respBuf);

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

void Dali::setLightPower(unsigned int channel, unsigned int power)
{
   std::stringstream stream;
   stream << "h" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << channel * 2;
   stream << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << power << "\n";
   // cout << "Send command: " << stream.str();
   commissioningCommunication(stream.str().c_str());
}


void Dali::communicateCommand(const char* cmd, int respN, char* resp)
{
   int i = 0;
   
   cout << "Sending command..." << endl;
   serialPuts(m_fd, cmd);
   cout << "Command sent" << endl;
   char versionString[12];
   for (i = 0; i<respN; i++)
   {
      versionString[i] = (char)serialGetchar(m_fd);
   }
   versionString[i] = 0;
   
   cout << "Command answer: " << versionString << endl;

   int remain = serialDataAvail(m_fd);
   
   cout << "There are " << remain << " bytes remaining in the serial buffer" << endl;
   
   if (remain > 0)
   {
      for (i = 0; i < remain; i++)
      {
         serialGetchar(m_fd);
      }
   }
   else if (remain == 0)
   {
      cout << "Lets consume one byte anyway." << endl;
      int c = serialGetchar(m_fd);

      cout << "I read: " << c << endl;
   }
   cout << "Remaining bytes consumed." << endl;
   
}


int Dali::commissioningCommunication(const char* cmd)
{
   int i = 0;
   
   delay(10);
   
   serialPuts(m_fd, cmd);

   char c = (char)serialGetchar(m_fd);
   
   if (c == 'N')
   {
      c = (char)serialGetchar(m_fd);
      // cout << "Found N, I consumed: " << (int)c << endl;

      if (cmd[0] == 't')
      {
         // cout << "This was sent twice, consume second response also" << endl;
         c = (char)serialGetchar(m_fd);
         
         if (c == 'N')
         {
            c = (char)serialGetchar(m_fd);
            // cout << "Found N again, I consumed: " << (int)c << endl;
         }
         else
         {
            // cout << "Expected N but found: " << (int)c << endl;
         }
      }
   
      // cout << "Is there any more data in the buffer?" << endl;
      // int ic = serialGetchar(m_fd);
      // cout << "ic is: " << ic << endl;
  

      return -1;
   }

   

   if (c == 'J')
   {
      char ans1 = (char)serialGetchar(m_fd);
      
      unsigned int ansInt1;
      std::stringstream ss1;
      ss1 << ans1;
      ss1 >> std::hex >> ansInt1;

      char ans2 = (char)serialGetchar(m_fd);

      unsigned int ansInt2;
      std::stringstream ss2;
      ss2 << ans2;
      ss2 >> std::hex >> ansInt2;
      
      unsigned int ans = ansInt1 + ansInt2 * 16;

      c = (char)serialGetchar(m_fd);
      // cout << "Found J, I consumed: " << (int)c << endl;

      return ans;
   }
   
   // cout << "Found nothing but this: " << (int)c << "'" << c << "'" << endl;
   return -1;
}

//#define COMMISSIONING_TEST

void Dali::sendSearchAddressHigh(unsigned int searchAddress)
{
   std::stringstream stream;
   stream << "hB1" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << ((searchAddress & 0xff0000) >> 16) << "\n";
   cout << "Send high address: " << stream.str().c_str();
   
   m_rxSearchAddress = (m_rxSearchAddress & 0xffff) | (searchAddress & 0xff0000);

#ifndef COMMISSIONING_TEST
   commissioningCommunication(stream.str().c_str());
#endif
}

void Dali::sendSearchAddressMid(unsigned int searchAddress)
{
   std::stringstream stream;
   stream << "hB3" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << ((searchAddress & 0xff00) >> 8) << "\n";
   cout << "Send mid address: " << stream.str().c_str();

   m_rxSearchAddress = (m_rxSearchAddress & 0xff00ff) | (searchAddress & 0xff00);

#ifndef COMMISSIONING_TEST
   commissioningCommunication(stream.str().c_str());
#endif
}

void Dali::sendSearchAddressLow(unsigned int searchAddress)
{
   std::stringstream stream;
   stream << "hB5" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << (searchAddress & 0xff) << "\n";
   cout << "Send low address: " << stream.str().c_str();

   m_rxSearchAddress = (m_rxSearchAddress & 0xffff00) | (searchAddress & 0xff);

#ifndef COMMISSIONING_TEST
   commissioningCommunication(stream.str().c_str());
#endif
}

bool Dali::compareSearchAddress(void)
{
   unsigned int ans = -1;

#ifdef COMMISSIONING_TEST
   if ((!m_withdrawn1 && (m_randomAddress1 <= m_rxSearchAddress)) || (!m_withdrawn2 && (m_randomAddress2 <= m_rxSearchAddress)))
   {
      ans = 0xff;
   }
#else   
   ans = commissioningCommunication("hA900\n");
   // cout << "Is there any more data in the buffer?" << endl;
   // int ic = serialGetchar(m_fd);
   // cout << "ic is: " << ic << endl;

#endif
  
   cout << "Answer: " << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << ans << endl << std::dec;

   return (ans == 0xff);
}

void Dali::queryShortAddress(void)
{
#ifndef COMMISSIONING_TEST
   unsigned int ans = commissioningCommunication("hBB00\n");
  
   cout << "Query short address: " << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << ans << endl << std::dec;
#endif
}


void Dali::withdraw(void)
{
   cout << "Withdraw matching device, using command: " << "hAB00\n";


#ifdef COMMISSIONING_TEST
   if (m_rxSearchAddress == m_randomAddress1)
   {
      m_withdrawn1 = true;
   }

   if (m_rxSearchAddress == m_randomAddress2)
   {
      m_withdrawn2 = true;
   }
#else
   commissioningCommunication("hAB00\n");
            
#endif

}

void Dali::commisioningProtocol(void)
{
   m_randomAddress1 = 0xdeadbe;
   m_randomAddress2 = 0x00dead;
   m_rxSearchAddress = 0;
   m_withdrawn1 = false;
   m_withdrawn2 = false;

   char respBuf[10];
   
   
#ifndef COMMISSIONING_TEST

   cout << "Send terminate..." << endl;
   commissioningCommunication("hA100\n");
   
   delay(500);


   cout << "Broadcast reset..." << endl;
   commissioningCommunication("t2000\n");
   
   delay(500);

   cout << "Send all off..." << endl;
   commissioningCommunication("hFF00\n");
   
   delay(500);


   // Below, with A500, I get all yes after query
   // but with A5FF I get all no after query

   cout << "Send initial twice..." << endl;
   commissioningCommunication("tA500\n");

   delay(500);
   
   cout << "Send randomize twice..." << endl;
   commissioningCommunication("tA700\n");

   delay(500);

#endif
   
   // Loop search address, start in the middle and go up or down until match
   unsigned int searchAddress = 0x800000;
   unsigned int thisBit = 0x800000;
   unsigned int nextBit = 0x400000;
   unsigned int lowestMatchAddress = 0x1000000;
   unsigned int highestEmptyAddress = 0x0000000;
   unsigned int shortAddress = 1;
   
   std::stringstream stream;
   bool done = false;
   
   int iterations = 0;
   
   
/*    // Do a dummy loop around of all address space
   searchAddress = 0;
   unsigned int bit = 1;
   
   while (!done)
   {
      delay(100);
      // Send searchAddress:
      cout << endl;
      cout << "Search address: " << std::setfill('0') << std::setw(6) << std::hex << std::uppercase <<  searchAddress << endl << std::dec;

      // High address
      sendSearchAddressHigh(searchAddress);

      // Mid address
      sendSearchAddressMid(searchAddress);
      
      // Low address
      sendSearchAddressLow(searchAddress);

      // Send compare command:
      cout << "Send compare command..." << endl;
      if (compareSearchAddress(m_fd))
      {
         cout << "Address match at: " << std::setfill('0') << std::setw(6) << std::hex << std::uppercase <<  searchAddress << endl << std::dec;
      }
      else
      {
         cout << "NAM: " << std::setfill('0') << std::setw(6) << std::hex << std::uppercase <<  searchAddress << endl << std::dec;
      }
      
      if (searchAddress >= 0xffffff)
      {
         done = true;
      }
      else if (searchAddress <= 0x400000)
      {
         searchAddress = bit;
         bit = bit * 2;
      }
      else if (searchAddress == 0x800000)
      {
         bit = 0x400000;
         searchAddress += bit;
         bit = bit / 2;
      }
      else
      {
         searchAddress += bit;
         bit = bit / 2;
      }
   }

 */



   

   // Loop until random address match:
   while ((!done) && (searchAddress > 0))
   {
      delay(100);
      // Send searchAddress:
      cout << endl;
      cout << "Lowest Match address: " << std::setfill('0') << std::setw(6) << std::hex << std::uppercase <<  lowestMatchAddress << endl << std::dec;
      cout << "Highest Empty address: " << std::setfill('0') << std::setw(6) << std::hex << std::uppercase <<  highestEmptyAddress << endl << std::dec;
      cout << "Search address: " << std::setfill('0') << std::setw(6) << std::hex << std::uppercase <<  searchAddress << endl << std::dec;

      // High address
      sendSearchAddressHigh(searchAddress);

      // Mid address
      sendSearchAddressMid(searchAddress);
      
      // Low address
      sendSearchAddressLow(searchAddress);

      // Send compare command:
      cout << "Send compare command..." << endl;
      if (compareSearchAddress())
      {
         // Someone is at this address or higher
         if (lowestMatchAddress > searchAddress)
         {
            lowestMatchAddress = searchAddress;
         }
         // Go down a bit
         searchAddress -= thisBit;
         searchAddress += nextBit;
         thisBit = nextBit;
         nextBit = nextBit >> 1;
      }
      else
      {
         // No answer, we are above any random address
         if (highestEmptyAddress < searchAddress)
         {
            highestEmptyAddress = searchAddress;
         }

         // Increase address
         searchAddress += nextBit;
         thisBit = nextBit;
         nextBit = nextBit >> 1;

      }
      
      iterations++;
      
      if (iterations > 24)
      {
         // Now we have an adress match
         cout << "Address match at: " << std::setfill('0') << std::setw(6) << std::hex << std::uppercase << lowestMatchAddress << endl << std::dec;
         
         // Reprogram the matching address
         
         // High address
         sendSearchAddressHigh(lowestMatchAddress);

         // Mid address
         sendSearchAddressMid(lowestMatchAddress);
         
         // Low address
         sendSearchAddressLow(lowestMatchAddress);
      
         stream << "hB7" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << shortAddress << "\n";
         cout << "Program short address: " << shortAddress << " to matching device, using command: " << stream.str();
#ifndef COMMISSIONING_TEST
         commissioningCommunication(stream.str().c_str());
#endif
         stream.clear();
         stream.str("");
         
         // Query short address of the matching device
         // queryShortAddress(m_fd);

         // Withdraw short address:
         withdraw();

         shortAddress += 2;
         
         if (shortAddress > 3)
         {
            done = true;
         }
         
         
         // Start binary search over
         searchAddress = 0x800000;
         thisBit = 0x800000;
         nextBit = 0x400000;
         lowestMatchAddress = 0x1000000;
         highestEmptyAddress = 0x0000000;

         iterations = 0;
      }
   }

   
   cout << "Commisioning complete!!!" << endl;
   
   cout << "Send terminate..." << endl;
   commissioningCommunication("hA100\n");
   
   delay(500);

   cout << "Send on command..." << endl;
   commissioningCommunication("hFF05\n");
   
   delay(500);
}



void Dali::queryChannel(int channel)
{
   cout << "Query " << channel << "..." << endl;
   
   int chanM = channel * 2 + 1;
   
   std::stringstream stream;
   stream << "h" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << chanM << "A0\n";
   
   // "h0390\n"
   cout << "Writing command: " << stream.str().c_str() << endl;
   serialPuts(m_fd, stream.str().c_str());
   
   char rc = (char)serialGetchar(m_fd);
   
   if (rc == 'J')
   {
      cout << "Channel " << channel << " exists. ---------------------------------" << endl;
      serialGetchar(m_fd);
      serialGetchar(m_fd);
      serialGetchar(m_fd);
   }
   else if (rc == 'N')
   {
      serialGetchar(m_fd);
   }
   else
   {
      cout << "Received '" << rc << "'" << endl;
   }

}

void Dali::terminate(void)
{
   serialClose(m_fd);
}

