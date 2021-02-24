#include <stdlib.h>

#include "ir.h"
#include <lirc_client.h>

using namespace std;

Ir::Ir()
{   
}

void Ir::initialize(void)
{
   // Set ir device and socket for sending
   system("./prepare_for_tx.sh");
}


void Ir::sendIr(const char* remote, const char* button)
{
   int fd;
   
   fd = lirc_get_local_socket(NULL, 0);
   
   if (fd < 0) {
      // Process error
   }
   if (lirc_send_one(fd, remote, button) == -1) {
      // Process errors
   };
}
