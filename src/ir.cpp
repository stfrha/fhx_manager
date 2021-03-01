#include <stdlib.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>


#include "ir.h"

using namespace std;

Ir::Ir()
{   
}

void Ir::initialize(void)
{
   // Set ir device and socket for sending
   system("./prepare_for_tx.sh");
}

int Ir::send_packet(lirc_cmd_ctx* ctx, int fd)
{
   int r;
   
   do {
      r = lirc_command_run(ctx, fd);
      if (r != 0 && r != EAGAIN)
         cout << "Error running command: " << strerror(r) << endl;
   } while (r == EAGAIN);

   return r == 0 ? 0 : -1;
}


void Ir::sendIrSequence(const char* command, const char* remote, const char* button)
{
   int fd;
   lirc_cmd_ctx ctx;
   int r;
   
   fd = lirc_get_local_socket(NULL, 0);
   
   if (fd < 0) {
      cout << "ERROR: Cannot open socket." << endl;
      return;
   }

   r = lirc_command_init(&ctx, "%s %s %s\n", command, remote, button);

   if (r != 0) 
   {
      cout << "Input too long" << endl;
      return;
   }

   if (send_packet(&ctx, fd) == -1)
   {
      return;
   }

   // if (lirc_send_one(fd, remote, button) == -1) {
   //    // Process errors
   // };
}

void Ir::sendIr(const char* remote, const char* button)
{
   sendIrSequence("SEND_START", remote, button);
   usleep(250000);
   sendIrSequence("SEND_STOP", remote, button);
}


