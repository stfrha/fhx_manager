
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <iomanip>

#include "comms.h"

using namespace std;

int socket_desc = 0;





int main(int argc, char *argv[])
{
   cout << "Welcome to ynca tester!" << endl;

   Comms comms;

   string command;

   cout << "Opening socket..." << endl;

   int socket_desc = comms.getYamahaSocket();

   cout << "Sending commmand..." << endl;
   
   comms.yamahaComm(socket_desc, "@MAIN:PWR=On\r\n");

   cout << "Calling recv..." << endl;
   
   comms.yamahaReceive(socket_desc);

   cout << "Closing socket..." << endl;

   comms.closeSocket();

   cout << "Repoening socket..." << endl;

   socket_desc = comms.getYamahaSocket();

   cout << "Calling recv again..." << endl;

   comms.yamahaReceive(socket_desc);

   while (true)
   {
      cout << "Input command, finish with X - enter." << endl;
      cin >> command;
      cout << "Command: " << command << endl;
      
      if (command[0] == 'X')
      {
         comms.closeSocket();
         return 0;
      }
      
      command += "\r\n";
      
      comms.yamahaComm(socket_desc, command.c_str());
   }

   return 0;
}
