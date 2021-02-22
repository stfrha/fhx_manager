#ifndef COMMS_H
#define COMMS_H

class Comms
{
private:
   int m_socket_desc;

   static void* receiverThread(void* threadId);


public:
   Comms();
   // Should probably have a destructor that stops all sockets
   
   int getYamahaSocket(void);
   void closeSocket(void);
   int yamahaComm(int socket_desc, const char* request);
   int yamahaReceive(int socket_desc);
};

#endif
