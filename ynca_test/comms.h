#ifndef COMMS_H
#define COMMS_H

class Comms
{
private:
   int getYamahaSocket(void);

public:
   Comms();
   // Should probably have a destructor that stops all sockets
   
   void yamahaClientComm(void);
   int yamahaComm(const char* request, char* reply, int replyMaxLen);
};

#endif
