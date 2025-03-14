
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

using namespace std;

// The message queue
pthread_mutex_t msgQueuMutex = PTHREAD_MUTEX_INITIALIZER;
vector<string> g_messageQueue;

// Conditional variable to indicate message in queue
pthread_cond_t g_cv;
pthread_mutex_t g_cvLock;

int main(int argc, char *argv[])
{
   cout << "Welcome to FHX-manager, now built with CMAKE and TLC59711!" << endl;

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
