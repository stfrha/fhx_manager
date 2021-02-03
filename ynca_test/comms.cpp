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

int Comms::getYamahaSocket(void)
{
   int socket_desc = 0;
   struct sockaddr_in server;
   char yamaha_reply[200];
   
   socket_desc = socket(AF_INET, SOCK_STREAM, 0);
   if (socket_desc == -1)
   {
      cout << "Could not create Yamaha client socket" << endl;
   }
   
   server.sin_addr.s_addr = inet_addr("192.168.0.113");
   server.sin_family = AF_INET;
   server.sin_port = htons(50000);
   
   if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
   {
      cout << "Yamaha client connection error" << endl;     
   }
   
   return socket_desc;
}

void Comms::yamahaClientComm(void)
{
   char yamaha_reply[200];

   cout << "Running yamaha comm test" << endl;
   
   int socket_desc = getYamahaSocket();
   
   char message[] = "@MAIN:PWR=?\r\n";

   cout << "Sending data..." << endl;
   
   if (send(socket_desc, message, strlen(message), 0) < 0)
   {
      cout << "Yamaha client send error" << endl;     
   }

   cout << "Receiving data..." << endl;
   
   int len = recv(socket_desc, yamaha_reply, 200, 0);
   if (len < 0 )
   {
      cout << "Yamaha client receive error" << endl;     
   }

   cout << "Received " << len << " bytes." << endl;
   
   cout << "Got reply: " << yamaha_reply << endl;
 
   close(socket_desc);
}

int Comms::yamahaComm(const char* request, char* reply, int replyMaxLen)
{
   int socket_desc = getYamahaSocket();
   if (send(socket_desc, request, strlen(request), 0) < 0)
   {
      cout << "Yamaha client send error" << endl;     
   }
   
   int len = recv(socket_desc, reply, replyMaxLen, 0);
   if (len < 0 )
   {
      cout << "Yamaha client receive error" << endl;     
   }

   cout << "Received " << len << " bytes." << endl;

   reply[len] = 0;
   
   cout << "Got reply: " << reply << endl;
 
   close(socket_desc);
   
   return len;
}



