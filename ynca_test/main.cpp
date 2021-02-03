
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <iomanip>

#include "comms.h"
#include "ynca.h"

using namespace std;

int main(int argc, char *argv[])
{
   cout << "Welcome to ynca tester!" << endl;

   Comms comms;
   Ynca ynca(&comms);

   string command;
   char reply[2000];

   while (true)
   {
      cout << "Input command, finish with enter." << endl;
      cin >> command;
      cout << "Command: " << command << endl;
      
      command += "\r\n";
      
      comms.yamahaComm(command.c_str(), reply, 2000);
   }

   return 0;
}
