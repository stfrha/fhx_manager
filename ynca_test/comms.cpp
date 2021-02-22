#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <vector>

// For threads
#include <pthread.h>

// For sockets
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

// For socket client
#include <netdb.h>

#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros 

#include <errno.h> 
#include <arpa/inet.h> //close 

#include "comms.h"

using namespace std;

// Global variables:

// The message queue
extern pthread_mutex_t msgQueuMutex;
extern vector<string> g_messageQueue;

// Conditional variable to indicate message in queue
extern pthread_cond_t g_cv;
extern pthread_mutex_t g_cvLock;

// Status as exposed globally from Controller class
extern string g_latestStatus;


// Both threads needs to agree on port number, 
// and both threads are static, i.e. make it global
int g_portNum = 51717;


void error(const char *msg)
{
   perror(msg);
   exit(1);
}

Comms::Comms()
{
}

void* Comms::receiverThread(void* somePointer)
{
   Comms* myself = (Comms*)somePointer;
   
   char reply[2000];

   while (true)
   {
      int len = recv(myself->m_socket_desc, reply, 2000, 0);
      
      if (len >= 0 )
      {
         cout << "Received " << len << " bytes." << endl;

         reply[len] = 0;
         
         cout << "Got reply: " << reply << endl;
      }

   }
    
}

int Comms::getYamahaSocket(void)
{
   m_socket_desc = 0;
   struct sockaddr_in server;
   char yamaha_reply[200];
   
   m_socket_desc = socket(AF_INET, SOCK_STREAM, 0);
   if (m_socket_desc == -1)
   {
      cout << "Could not create Yamaha client socket" << endl;
   }
   
   server.sin_addr.s_addr = inet_addr("192.168.0.113");
   server.sin_family = AF_INET;
   server.sin_port = htons(50000);
   
   if (connect(m_socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
   {
      cout << "Yamaha client connection error" << endl;     
   }

   // pthread_t threadId;
   // 
   // int result = pthread_create(&threadId, NULL, receiverThread, (void*)this);
   // if (result)
   // {
   //    cout << "Receiver thread could not be created, " << result << endl;
   //    exit(1);
   // }
   
   return m_socket_desc;
}

void Comms::closeSocket(void)
{
   close(m_socket_desc);
}

int Comms::yamahaComm(int socket_desc, const char* request)
{
   if (send(socket_desc, request, strlen(request), 0) < 0)
   {
      cout << "Yamaha client send error" << endl;     
   }
   
   return 0;
}

int Comms::yamahaReceive(int socket_desc)
{
   char reply[20];

   int len = recv(socket_desc, reply, 20, 0);
   
   if (len >= 0 )
   {
      cout << "Received " << len << " bytes." << endl;

      reply[len] = 0;
      
      cout << "Got reply: " << reply << endl;
   }

}


