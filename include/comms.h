#ifndef COMMS_H
#define COMMS_H


#include "controller.h"

class Comms
{
private:
   Controller * m_cntrl;



   static void* serverThread(void* threadId);
   static void handleMessage(Controller* cntrl, int socketFd, char* buffer, int length);

   int getYamahaSocket(void);

public:
   Comms();

   void initializeComms(Controller* cntrl);

   // Should probably have a destructor that stops all sockets
   
   void yamahaClientComm(void);
   int yamahaComm(const char* request, char* reply, int replyMaxLen, bool purge = false);

};

#endif
