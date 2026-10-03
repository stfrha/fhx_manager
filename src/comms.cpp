#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <vector>
#include <unistd.h>


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
#include "commands.h"

using namespace std;

// Global variables:

// The message queue
// extern pthread_mutex_t msgQueuMutex;
// extern vector<string> g_messageQueue;

// Conditional variable to indicate message in queue
// extern pthread_cond_t g_cv;
// extern pthread_mutex_t g_cvLock;

// Status as exposed globally from Controller class
// extern string g_latestStatus;


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

void Comms::initializeComms(Controller* cntrl)
{
   m_cntrl = cntrl;
   pthread_t threadId;

   int result = pthread_create(&threadId, NULL, serverThread, (void*)cntrl);
   if (result)
   {
      cout << "Server thread could not be created, " << result << endl;
      exit(1);
   }
}

void Comms::sendCommand(Controller* cntrl, int socketFd, const CommandStruct& cs)
{
   int n = 0;
   string latestStatus = cntrl->executeCommand(cs);
   n = write(socketFd, latestStatus.c_str(), latestStatus.length());
   if (n < 0)
   {
      error("ERROR writing to socket");
   }
}

void Comms::handleMessage(Controller* cntrl, int socketFd, char* buffer, int length)
{

   // Check if the buffer contains at least one message (no broken)
   if (length < 4)
   {
      // At least one broken message, discard the whole buffer
      cout << "At least one broken message, discard the whole buffer" << endl;
      cout << "-- Length is: " << length << endl;
      cout << "-- Message data is: " << buffer << endl;

      sendCommand(cntrl, socketFd, CommandStruct(t_commands::statusReq, ""));

      return;
   }

   // Parse (possibly multiple) messages
   int n = 0;
   
   while (n < length)
   {
      CommandStruct cs;
      int magic;
      int msgLen;
      int id;
      int startN = n;
      
      magic = buffer[n++];
      
      if (magic != CommandStruct::c_magic)
      {
         // At least one broken message, discard the whole buffer
         cout << "At least one broken message, did not find magic word, discard the whole buffer" << endl;
         cout << "-- Length is: " << length << endl;
         cout << "-- Message data is: " << buffer << endl;

         sendCommand(cntrl, socketFd, CommandStruct(t_commands::statusReq, ""));

         return;
      }

      msgLen = buffer[n++];
      if (msgLen > 4)
      {
         // There is an argument, we want length to be at least equal to msgLen
         if (length < msgLen + startN)
         {
            // At least one broken message, discard the whole buffer
            cout << "At least one broken message, discard the whole buffer" << endl;
            cout << "-- Length is: " << length << endl;
            cout << "-- Message data is: " << buffer << endl;
            
            sendCommand(cntrl, socketFd, CommandStruct(t_commands::statusReq, ""));
            
            return;

         }
      }

      unsigned int tId = buffer[n++] * 256 + buffer[n++] ;
      cs.m_id = (t_commands)tId;
      cs.m_argument = "";
      
      for (int i = 0; i < msgLen - 4; i++)
      {
         cs.m_argument += buffer[n++];
      }
      
      sendCommand(cntrl, socketFd, cs);
   }
}

void* Comms::serverThread(void* cntrlPointer)
{
   Controller* cntrl = (Controller*)cntrlPointer;

   int masterSockfd;
   int newSockfd;
   socklen_t clilen;
   char buffer[1024];
   struct sockaddr_in serv_addr;
   struct sockaddr_in cli_addr;
   int n;

   int clientSocket[30];
   int max_clients = 30;
   int activity;
   int i;
   int valread;
   int sd;
   int max_sd;

   //initialise all clientSocket[] to 0 so not checked 
   for (i = 0; i < max_clients; i++)
   {
      clientSocket[i] = 0;
   }

   //set of socket descriptors 
   fd_set readfds;

   masterSockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (masterSockfd < 0)
   {
      error("ERROR opening socket");
   }

   bzero((char *)&serv_addr, sizeof(serv_addr));
   g_portNum = 51717;
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(g_portNum);

   while (bind(masterSockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
   {
      if (errno == EADDRINUSE)
      {
         cout << "Port " << g_portNum << " is already in use. ";
         g_portNum++;
         serv_addr.sin_port = htons(g_portNum);
         cout << "Trying " << g_portNum << " instead." << endl;
      }
      else
      {
         error("ERROR on binding");
      }
   }

   ofstream sockectConfigFile("socket_config.txt");
   if (sockectConfigFile.is_open())
   {
      sockectConfigFile << g_portNum;
      sockectConfigFile.close();
   }

   listen(masterSockfd, 5);
   clilen = sizeof(cli_addr);

   while (true)
   {
      //clear the socket set 
      FD_ZERO(&readfds);

      //add master socket to set 
      FD_SET(masterSockfd, &readfds);
      max_sd = masterSockfd;

      //add child sockets to set 
      for (i = 0; i < max_clients; i++)
      {
         //socket descriptor 
         sd = clientSocket[i];

         //if valid socket descriptor then add to read list 
         if (sd > 0)
         {
            FD_SET(sd, &readfds);
         }

         //highest file descriptor number, need it for the select function 
         if (sd > max_sd)
         {
            max_sd = sd;
         }
      }

      //wait for an activity on one of the sockets , timeout is NULL , 
      //so wait indefinitely 
      activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

      if ((activity < 0) && (errno != EINTR))
      {
         cout << "SERVER: Error in select!" << endl;
         exit(1);
      }

      //If something happened on the master socket , 
      //then its an incoming connection 
      if (FD_ISSET(masterSockfd, &readfds))
      {

         newSockfd = accept(masterSockfd, (struct sockaddr *) &cli_addr, &clilen);
         if (newSockfd < 0)
         {
            error("ERROR on accept");
         }

         //inform user of socket number - used in send and receive commands 
         printf("New connection , socket fd is %d , ip is : %s , port : %d\n", newSockfd, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

         bzero(buffer, 256);
         // shouldn't below be 256?
         n = read(newSockfd, buffer, 255);
         if (n < 0)
         {
            error("ERROR reading from socket");
         }

         handleMessage(cntrl, newSockfd, buffer, n);

         //add new socket to array of sockets 
         for (i = 0; i < max_clients; i++)
         {
            //if position is empty 
            if (clientSocket[i] == 0)
            {
               clientSocket[i] = newSockfd;
               break;
            }
         }
      }

      //else its some IO operation on some other socket 
      for (i = 0; i < max_clients; i++)
      {
         sd = clientSocket[i];

         if (FD_ISSET(sd, &readfds))
         {
            //Check if it was for closing , and also read the 
            //incoming message 
            if ((valread = read(sd, buffer, 1024)) == 0)
            {
               //Somebody disconnected , get his details and print 
               getpeername(sd, (struct sockaddr*)&cli_addr, &clilen);
               printf("Host disconnected , ip %s , port %d \n",
                  inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

               //Close the socket and mark as 0 in list for reuse 
               close(sd);
               clientSocket[i] = 0;
            }

            // Assess the message that came in 
            else
            {
               // Terminate receiver string after the number of bytes received.
               buffer[valread] = 0;

               handleMessage(cntrl, sd, buffer, valread);

            }
         }
      }
   }
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
   
   server.sin_addr.s_addr = inet_addr("192.168.0.203");
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

int Comms::yamahaComm(const char* request, char* reply, int replyMaxLen, bool purge)
{
   cout << "Starting Yamaha comm!" << endl;

   cout << "Sending message: '" << request << "'" << endl;
   
   int socket_desc = getYamahaSocket();
   if (send(socket_desc, request, strlen(request), 0) < 0)
   {
      cout << "Yamaha client send error" << endl;     
   }

   cout << "Calling recv!" << endl;
   
   int len = recv(socket_desc, reply, replyMaxLen, 0);
   if (len < 0 )
   {
      cout << "Yamaha client receive error" << endl;     
   }
   else
   {
      cout << "Received " << len << " bytes." << endl;

      reply[len] = 0;
      
      cout << "Got reply: " << reply << endl;
   }
   
   if (purge)
   {
      usleep(500000);
      len = recv(socket_desc, reply, replyMaxLen, 0);
      cout << "Purged: " << len << " characters." << endl;
   }
 
   close(socket_desc);

   cout << "Yamaha comm finished" << endl;
   
   return len;
}



