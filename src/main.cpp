
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <iomanip>
#include <ctime>
#include <cstdlib>

#include "comms.h"
#include "controller.h"
#include "ynca.h"
#include "dali.h"

using namespace std;

// The message queue
pthread_mutex_t msgQueuMutex = PTHREAD_MUTEX_INITIALIZER;
vector<string> g_messageQueue;

// Conditional variable to indicate message in queue
pthread_cond_t g_cv;
pthread_mutex_t g_cvLock;

int main(int argc, char *argv[])
{
   // One-shot commissioning of a replaced DALI driver:
   //   ./fhx_manager --commission <short address>
   // Stop the normal fhx_manager service first so the serial port is free.
   if (argc == 3 && strcmp(argv[1], "--commission") == 0)
   {
      int addr = atoi(argv[2]);
      if (addr < 0 || addr > 63)
      {
         cout << "Short address must be 0..63" << endl;
         return 1;
      }

      Dali dali;
      if (!dali.initializeDali())
      {
         return 1;
      }
      cout << "Commissioning new driver to short address " << addr << endl;
      dali.commisioningProtocol(addr);
      dali.terminate();
      return 0;
   }

   cout << "Welcome to FHX-manager!" << endl;

   srand(time(0));

   Comms comms;
   Ynca ynca(&comms);
   Controller cntrl(&comms, &ynca);

   cntrl.initializeController();
   comms.initializeComms(&cntrl);


   while (true)
   {
      delay(1000);
   }

   return 0;

}
