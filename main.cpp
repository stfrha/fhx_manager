
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <iomanip>

#include <wiringPi.h>
#include <softPwm.h>
#include <wiringSerial.h>

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

#define BLUE_LED 25
#define RED_LED 28
#define GREEN_LED 29

void communicateCommand(int fd, const char* cmd, int respN, char* resp)
{
   int i = 0;
   
   cout << "Sending command..." << endl;
   serialPuts(fd, cmd);
   cout << "Command sent" << endl;
   char versionString[respN+2];
   for (i = 0; i<respN; i++)
   {
      versionString[i] = (char)serialGetchar(fd);
   }
   versionString[i] = 0;
   
   cout << "Command answer: " << versionString << endl;

   int remain = serialDataAvail(fd);
   
   cout << "There are " << remain << " bytes remaining in the serial buffer" << endl;
   
   if (remain > 0)
   {
      for (i = 0; i < remain; i++)
      {
         serialGetchar(fd);
      }
   }
   else if (remain == 0)
   {
      cout << "Lets consume one byte anyway." << endl;
      int c = serialGetchar(fd);

      cout << "I read: " << c << endl;
   }
   cout << "Remaining bytes consumed." << endl;
   
}


int commissioningCommunication(int fd, const char* cmd)
{
   cout << "Sending command: " << cmd;
   int i = 0;
   
   delay(10);
   
   serialPuts(fd, cmd);

   char c = (char)serialGetchar(fd);
   
   if (c == 'N')
   {
      c = (char)serialGetchar(fd);
      cout << "Found N, I consumed: " << (int)c << endl;

      if (cmd[0] == 't')
      {
         cout << "This was sent twice, consume second response also" << endl;
         c = (char)serialGetchar(fd);
         
         if (c == 'N')
         {
            c = (char)serialGetchar(fd);
            cout << "Found N again, I consumed: " << (int)c << endl;
         }
         else
         {
            cout << "Expected N but found: " << (int)c << endl;
         }
      }
   
      // cout << "Is there any more data in the buffer?" << endl;
      // int ic = serialGetchar(fd);
      // cout << "ic is: " << ic << endl;
  

      return -1;
   }

   

   if (c == 'J')
   {
      char ans1 = (char)serialGetchar(fd);
      
      unsigned int ansInt1;
      std::stringstream ss1;
      ss1 << ans1;
      ss1 >> std::hex >> ansInt1;

      char ans2 = (char)serialGetchar(fd);

      unsigned int ansInt2;
      std::stringstream ss2;
      ss2 << ans2;
      ss2 >> std::hex >> ansInt2;
      
      unsigned int ans = ansInt1 + ansInt2 * 16;

      c = (char)serialGetchar(fd);
      cout << "Found J, I consumed: " << (int)c << endl;

      return ans;
   }
   
   cout << "Found nothing but this: " << (int)c << "'" << c << "'" << endl;
   return -1;
}

//#define COMMISSIONING_TEST

unsigned int randomAddress1 = 0xdeadbe;
unsigned int randomAddress2 = 0x00dead;
unsigned int rxSearchAddress = 0;
bool withdrawn1 = false;
bool withdrawn2 = false;


void sendSearchAddressHigh(int fd, unsigned int searchAddress)
{
   std::stringstream stream;
   stream << "hB1" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << ((searchAddress & 0xff0000) >> 16) << "\n";
   cout << "Send high address: " << stream.str().c_str();
   
   rxSearchAddress = (rxSearchAddress & 0xffff) | (searchAddress & 0xff0000);

#ifndef COMMISSIONING_TEST
   commissioningCommunication(fd, stream.str().c_str());
#endif
}

void sendSearchAddressMid(int fd, unsigned int searchAddress)
{
   std::stringstream stream;
   stream << "hB3" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << ((searchAddress & 0xff00) >> 8) << "\n";
   cout << "Send mid address: " << stream.str().c_str();

   rxSearchAddress = (rxSearchAddress & 0xff00ff) | (searchAddress & 0xff00);

#ifndef COMMISSIONING_TEST
   commissioningCommunication(fd, stream.str().c_str());
#endif
}

void sendSearchAddressLow(int fd, unsigned int searchAddress)
{
   std::stringstream stream;
   stream << "hB5" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << (searchAddress & 0xff) << "\n";
   cout << "Send low address: " << stream.str().c_str();

   rxSearchAddress = (rxSearchAddress & 0xffff00) | (searchAddress & 0xff);

#ifndef COMMISSIONING_TEST
   commissioningCommunication(fd, stream.str().c_str());
#endif
}

bool compareSearchAddress(int fd)
{
   unsigned int ans = -1;

#ifdef COMMISSIONING_TEST
   if ((!withdrawn1 && (randomAddress1 <= rxSearchAddress)) || (!withdrawn2 && (randomAddress2 <= rxSearchAddress)))
   {
      ans = 0xff;
   }
#else   
   ans = commissioningCommunication(fd, "hA900\n");
   // cout << "Is there any more data in the buffer?" << endl;
   // int ic = serialGetchar(fd);
   // cout << "ic is: " << ic << endl;

#endif
  
   cout << "Answer: " << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << ans << endl << std::dec;

   return (ans == 0xff);
}

void queryShortAddress(int fd)
{
#ifndef COMMISSIONING_TEST
   unsigned int ans = commissioningCommunication(fd, "hBB00\n");
  
   cout << "Query short address: " << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << ans << endl << std::dec;
#endif
}


void withdraw(int fd)
{
   cout << "Withdraw matching device, using command: " << "hAB00\n";


#ifdef COMMISSIONING_TEST
   if (rxSearchAddress == randomAddress1)
   {
      withdrawn1 = true;
   }

   if (rxSearchAddress == randomAddress2)
   {
      withdrawn2 = true;
   }
#else
   commissioningCommunication(fd, "hAB00\n");
            
#endif

}

void commisioningProtocol(int fd)
{
   char respBuf[10];
   
   
#ifndef COMMISSIONING_TEST

   cout << "Send terminate..." << endl;
   commissioningCommunication(fd, "hA100\n");
   
   delay(500);


   cout << "Broadcast reset..." << endl;
   commissioningCommunication(fd, "t2000\n");
   
   delay(500);

   cout << "Send all off..." << endl;
   commissioningCommunication(fd, "hFF00\n");
   
   delay(500);


   // Below, with A500, I get all yes after query
   // but with A5FF I get all no after query

   cout << "Send initial twice..." << endl;
   commissioningCommunication(fd, "tA500\n");

   delay(500);
   
   cout << "Send randomize twice..." << endl;
   commissioningCommunication(fd, "tA700\n");

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
      sendSearchAddressHigh(fd, searchAddress);

      // Mid address
      sendSearchAddressMid(fd, searchAddress);
      
      // Low address
      sendSearchAddressLow(fd, searchAddress);

      // Send compare command:
      cout << "Send compare command..." << endl;
      if (compareSearchAddress(fd))
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
      sendSearchAddressHigh(fd, searchAddress);

      // Mid address
      sendSearchAddressMid(fd, searchAddress);
      
      // Low address
      sendSearchAddressLow(fd, searchAddress);

      // Send compare command:
      cout << "Send compare command..." << endl;
      if (compareSearchAddress(fd))
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
         sendSearchAddressHigh(fd, lowestMatchAddress);

         // Mid address
         sendSearchAddressMid(fd, lowestMatchAddress);
         
         // Low address
         sendSearchAddressLow(fd, lowestMatchAddress);
      
         stream << "hB7" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << shortAddress << "\n";
         cout << "Program short address: " << shortAddress << " to matching device, using command: " << stream.str();
#ifndef COMMISSIONING_TEST
         commissioningCommunication(fd, stream.str().c_str());
#endif
         stream.clear();
         stream.str("");
         
         // Query short address of the matching device
         // queryShortAddress(fd);

         // Withdraw short address:
         withdraw(fd);

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
   commissioningCommunication(fd, "hA100\n");
   
   delay(500);

   cout << "Send on command..." << endl;
   commissioningCommunication(fd, "hFF05\n");
   
   delay(500);
}



void queryChannel(int fd, int channel)
{
   cout << "Query " << channel << "..." << endl;
   
   int chanM = channel * 2 + 1;
   
   std::stringstream stream;
   stream << "h" << std::setfill('0') << std::setw(2) << std::hex << std::uppercase << chanM << "A0\n";
   
   // "h0390\n"
   cout << "Writing command: " << stream.str().c_str() << endl;
   serialPuts(fd, stream.str().c_str());
   
   char rc = (char)serialGetchar(fd);
   
   if (rc == 'J')
   {
      cout << "Channel " << channel << " exists. ---------------------------------" << endl;
      serialGetchar(fd);
      serialGetchar(fd);
      serialGetchar(fd);
   }
   else if (rc == 'N')
   {
      serialGetchar(fd);
   }
   else
   {
      cout << "Received '" << rc << "'" << endl;
   }

}

int main(int argc, char *argv[])
{
   char respBuf[200];

   cout << "Welcome to FHX-manager!" << endl;
   
   int fd = serialOpen("/dev/serial0", 19200);
   
   if (fd < 0)
   {
      fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
      return 1 ;
   }
   
   if (wiringPiSetup () == -1)
   {
      fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
      return 1 ;
   }

   pinMode(25, OUTPUT);
   pinMode(28, OUTPUT);
   pinMode(29, OUTPUT);

   digitalWrite(25, HIGH);
   digitalWrite(28, HIGH);
   digitalWrite(29, HIGH);
   
   softPwmCreate(BLUE_LED, 0, 100);

   // Test serial and DALI interface
   // write version request
   cout << "Sending version request..." << endl;
   communicateCommand(fd, "v\n", 7, respBuf);

   cout << "Sending power request..." << endl;
   communicateCommand(fd, "p\n", 8, respBuf);

   cout << "Sending power status request..." << endl;
   communicateCommand(fd, "d\n", 3, respBuf);

   // for (int i = 0; i < 64; i++)
   // {
   //    queryChannel(fd, i);
   // }

   cout << "Let's make sure the receiver buffer is empty, it takes some time..." << endl;
   int l = serialDataAvail(fd);
   cout << "There are " << l << " characters in in-buffer." << endl;

   for (int i = 0; i < l; i++)
   {
      serialGetchar(fd);
   }
   
   cout << "In-buffer is empty." << endl;

   
   commisioningProtocol(fd);

   serialClose(fd);
   
   while (true)
   {
      for (int i = 0; i < 100 ; i++)
      {
         softPwmWrite(BLUE_LED, i);
         delay(15);
      }
      for (int i = 100; i >= 0 ; i--)
      {
         softPwmWrite(BLUE_LED, i);
         delay(15);
      }
   }
   
   return 0;
}
