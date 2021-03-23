#ifndef IR_H
#define IR_H

#include "lirc_client.h"


class Ir
{
private:
   int send_packet(lirc_cmd_ctx* ctx, int fd);
   void sendIrSequence(const char* command, const char* remote, const char* button);
   
public:   
   Ir();

   void initialize(void);

   void sendIr(const char* remote, const char* button);
};


#endif


