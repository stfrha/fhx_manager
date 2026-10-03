#include <stdlib.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <iomanip>

#include <curl/curl.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

#include "ircc.h"
#include "base64.h"

using namespace std;

Ircc::Ircc()
{   

   // Init ircc array of commands
   m_irccCmnds.push_back("AAAAAwAAHFoAAAA9Aw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAA5Aw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAA6Aw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAA8Aw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAA7Aw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAABCAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAA/Aw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAABDAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAAAAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAABAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAACAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAADAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAAEAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAAFAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAAGAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAAHAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAAIAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAAJAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAAVAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAABBAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAABkAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAABjAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAABeAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAABpAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAABmAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAABnAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAABoAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAAaAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAAYAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAAZAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAAbAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAAcAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAABXAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAABWAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAB2Aw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAB1Aw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAABlAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAAsAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAApAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAAAWAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAABKAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAABLAw==");
   m_irccCmnds.push_back("AAAAAwAAHFoAAABNAw==");
   
}

void Ircc::initialize(void)
{
}

void Ircc::getDial(void)
{
   cout << "Requesting Dial.xml..." << endl;

   CURL *curl;
   CURLcode res;
   struct curl_slist *list = NULL;

   curl = curl_easy_init();

   if(curl) 
   {
      curl_easy_setopt(curl, CURLOPT_URL, "192.168.0.161:50201/dial.xml");
      list = curl_slist_append(list, "Connection: close");
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
      curl_easy_setopt(curl, CURLOPT_USERAGENT, "FHX Manager/0.1 (Raspberry Pi 3b+)");
      curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip");
 
      res = curl_easy_perform(curl);
      if(res != CURLE_OK)
         fprintf(stderr, "curl_easy_perform() failed: %s\n",
         curl_easy_strerror(res));

      /* always cleanup */ 
      curl_easy_cleanup(curl);
   }
}

void Ircc::getIrcc(void)
{
   cout << "Requesting Ircc.xml..." << endl;

   CURL *curl;
   CURLcode res;
   struct curl_slist *list = NULL;

   curl = curl_easy_init();

   if(curl) 
   {
      curl_easy_setopt(curl, CURLOPT_URL, "192.168.0.161:50001/Ircc.xml");
      list = curl_slist_append(list, "Connection: close");
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
      curl_easy_setopt(curl, CURLOPT_USERAGENT, "FHX Manager/0.1 (Raspberry Pi 3b+)");
      curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip");
 
      res = curl_easy_perform(curl);
      if(res != CURLE_OK)
         fprintf(stderr, "curl_easy_perform() failed: %s\n",
         curl_easy_strerror(res));

      /* always cleanup */ 
      curl_easy_cleanup(curl);
   }
}

void Ircc::getDmr(void)
{
   cout << "Requesting Dmr.xml..." << endl;

   CURL *curl;
   CURLcode res;
   struct curl_slist *list = NULL;

   curl = curl_easy_init();

   if(curl) 
   {
      curl_easy_setopt(curl, CURLOPT_URL, "192.168.0.161:52323/dmr.xml");
      curl_easy_setopt(curl, CURLOPT_USERAGENT, "UPnP/1.0");
      list = curl_slist_append(list, "X-AV-Client-Info: av=5.0; cn=\"Fredrik Hoffman\"; mn=\"FHX Manager\"; mv=\"0.1.0\";");
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
 
      res = curl_easy_perform(curl);
      if(res != CURLE_OK)
         fprintf(stderr, "curl_easy_perform() failed: %s\n",
         curl_easy_strerror(res));

      /* always cleanup */ 
      curl_easy_cleanup(curl);
   }
}

void Ircc::registerNoAuth(void)
{
   cout << "Registering without auth" << endl;

   CURL *curl;
   CURLcode res;
   struct curl_slist *list = NULL;

   curl = curl_easy_init();

   // Gather header strings.
   
   string registerArg = "register?name=BiorummetRPi+FHX_Manager&registrationType=initial&deviceId=FHX_Manager%3A";
   // Get macAddress
   string macAddress = "b8-27-eb-24-95-16";
   string trailingArg = "&wolSupport=true ";

   registerArg = registerArg + macAddress + trailingArg;

   cout << registerArg << endl;

   string url = "192.168.0.161:50002/" + registerArg;

   if(curl) 
   {
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      list = curl_slist_append(list, string("X-CERS-DEVICE_ID: FHX_Manager:" + macAddress).c_str());
      list = curl_slist_append(list, "X-CERS-DEVICE_INFO: Raspbian/FHX_Manager/BiorummetRPi");
      list = curl_slist_append(list, "Connection: close");
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
      curl_easy_setopt(curl, CURLOPT_USERAGENT, "FHX Manager/0.1 (Raspberry Pi 3b+)");
      curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip");

      res = curl_easy_perform(curl);
      if(res != CURLE_OK)
         fprintf(stderr, "curl_easy_perform() failed: %s\n",
         curl_easy_strerror(res));

      /* always cleanup */ 
      curl_easy_cleanup(curl);
   }
}

void Ircc::registerAuth(string authStr)
{
   cout << "Registering with auth" << endl;

   CURL *curl;
   CURLcode res;
   struct curl_slist *list = NULL;

   curl = curl_easy_init();

   // Gather header strings.
   
   string registerArg = "register?name=BiorummetRPi+FHX_Manager&registrationType=initial&deviceId=FHX_Manager%3A";
   // Get macAddress
   string macAddress = "b8-27-eb-24-95-16";
   string trailingArg = "&wolSupport=true ";

   registerArg = registerArg + macAddress + trailingArg;

   cout << registerArg << endl;

   string url = "192.168.0.161:50002/" + registerArg;

   if(curl) 
   {
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      list = curl_slist_append(list, string("X-CERS-DEVICE_ID: FHX_Manager:" + macAddress).c_str());
      list = curl_slist_append(list, string("Authorization: Basic " + authStr).c_str());
      list = curl_slist_append(list, "X-CERS-DEVICE_INFO: Raspbian/FHX_Manager/BiorummetRPi");
      list = curl_slist_append(list, "Connection: close");
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
      curl_easy_setopt(curl, CURLOPT_USERAGENT, "FHX Manager/0.1 (Raspberry Pi 3b+)");
      curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip");

      res = curl_easy_perform(curl);
      if(res != CURLE_OK)
         fprintf(stderr, "curl_easy_perform() failed: %s\n",
         curl_easy_strerror(res));

      /* always cleanup */ 
      curl_easy_cleanup(curl);
   }
}

void Ircc::sendPause(void)
// AAAAAwAAHFoAAAAZAw
{
   cout << "Send Pause IRCC" << endl;

   CURL *curl;
   CURLcode res;
   struct curl_slist *list = NULL;

   curl = curl_easy_init();

   // Gather header strings.
   
   string url = "192.168.0.161:50001/upnp/control/IRCC";
   string post = "<?xml version=\"1.0\"?><s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body><u:X_SendIRCC xmlns:u=\"urn:schemas-sony-com:service:IRCC:1\"><IRCCCode>AAAAAwAAHFoAAAAZAw==</IRCCCode></u:X_SendIRCC></s:Body></s:Envelope>";

// <?xml version="1.0"?><s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/" s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"><s:Body><u:X_SendIRCC xmlns:u="urn:schemas-sony-com:service:IRCC:1"><IRCCCode>AAAAAwAAHFoAAAAZAw==</IRCCCode></u:X_SendIRCC></s:Body></s:Envelope>



   if(curl) 
   {
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      list = curl_slist_append(list, "soapaction: \"urn:schemas-sony-com:service:IRCC:1#X_SendIRCC\"");
      list = curl_slist_append(list, "content-type: text/xml; charset=utf-8");
      list = curl_slist_append(list, "Connection: close");
      list = curl_slist_append(list, "Content-Length: 291");
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
      curl_easy_setopt(curl, CURLOPT_USERAGENT, "FHX Manager/0.1 (Raspberry Pi 3b+)");
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
      curl_easy_setopt(curl, CURLOPT_HEADER, 0);
      curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip");
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post.c_str());
      
      res = curl_easy_perform(curl);
      if(res != CURLE_OK)
         fprintf(stderr, "curl_easy_perform() failed: %s\n",
         curl_easy_strerror(res));

      /* always cleanup */ 
      curl_easy_cleanup(curl);
      curl_slist_free_all(list);
   }
}

void Ircc::sendCode(irccCode_t code)
{
   cout << "Send IRCC" << endl;

   CURL *curl;
   CURLcode res;
   struct curl_slist *list = NULL;

   curl = curl_easy_init();

   // Gather header strings.
   string url = "192.168.0.161:50001/upnp/control/IRCC";
   string post = "<?xml version=\"1.0\"?><s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body><u:X_SendIRCC xmlns:u=\"urn:schemas-sony-com:service:IRCC:1\"><IRCCCode>" + m_irccCmnds[(int)code] + "</IRCCCode></u:X_SendIRCC></s:Body></s:Envelope>";

   if(curl) 
   {
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      list = curl_slist_append(list, "soapaction: \"urn:schemas-sony-com:service:IRCC:1#X_SendIRCC\"");
      list = curl_slist_append(list, "content-type: text/xml; charset=utf-8");
      list = curl_slist_append(list, "Connection: close");
      list = curl_slist_append(list, "Content-Length: 291");
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
      curl_easy_setopt(curl, CURLOPT_USERAGENT, "FHX Manager/0.1 (Raspberry Pi 3b+)");
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
      curl_easy_setopt(curl, CURLOPT_HEADER, 0);
      curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip");
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post.c_str());
      
      res = curl_easy_perform(curl);
      if(res != CURLE_OK)
         fprintf(stderr, "curl_easy_perform() failed: %s\n",
         curl_easy_strerror(res));

      /* always cleanup */ 
      curl_easy_cleanup(curl);
      curl_slist_free_all(list);
   }
}

void Ircc::sendPlay(void)
// AAAAAwAAHFoAAAAaAw
{
   cout << "Send Play IRCC" << endl;

   CURL *curl;
   CURLcode res;
   struct curl_slist *list = NULL;

   curl = curl_easy_init();

   // Gather header strings.
   
   string url = "192.168.0.161:50001/upnp/control/IRCC";
   string post = "<?xml version=\"1.0\"?><s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body><u:X_SendIRCC xmlns:u=\"urn:schemas-sony-com:service:IRCC:1\"><IRCCCode>AAAAAwAAHFoAAAAaAw==</IRCCCode></u:X_SendIRCC></s:Body></s:Envelope>";

// <?xml version="1.0"?><s:Envelope xmlns:s="http://schemas.xmlsoap.org/soap/envelope/" s:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/"><s:Body><u:X_SendIRCC xmlns:u="urn:schemas-sony-com:service:IRCC:1"><IRCCCode>AAAAAwAAHFoAAAAaAw==</IRCCCode></u:X_SendIRCC></s:Body></s:Envelope>



   if(curl) 
   {
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      list = curl_slist_append(list, "soapaction: \"urn:schemas-sony-com:service:IRCC:1#X_SendIRCC\"");
      list = curl_slist_append(list, "content-type: text/xml; charset=utf-8");
      list = curl_slist_append(list, "Connection: close");
      list = curl_slist_append(list, "Content-Length: 291");
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
      curl_easy_setopt(curl, CURLOPT_USERAGENT, "FHX Manager/0.1 (Raspberry Pi 3b+)");
      curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
      curl_easy_setopt(curl, CURLOPT_HEADER, 0);
      curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip");
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post.c_str());
      
      res = curl_easy_perform(curl);
      if(res != CURLE_OK)
         fprintf(stderr, "curl_easy_perform() failed: %s\n",
         curl_easy_strerror(res));

      /* always cleanup */ 
      curl_easy_cleanup(curl);
      curl_slist_free_all(list);
   }
}

void Ircc::wakeOnLan(void)
{
   int sock;
   int optval = 1;
   unsigned char macAddr[6];

   string macAddress = "14:3f:a6:08:86:7b";

   for (int i = 0; i < 5; i++)
   {
      int l = macAddress.find_first_of(':');
      string adrByte = macAddress.substr(0, l);
      macAddr[i] = (unsigned char) strtol(adrByte.c_str(), NULL, 16);
      macAddress = macAddress.substr(l+1);
      cout << "MAC byte (decimal): " << (int)macAddr[i] << endl;
   }
   macAddr[5] = (unsigned char) strtol(macAddress.c_str(), NULL, 16);
   cout << "MAC byte (decimal): " << (int)macAddr[5] << endl;

   if(( sock = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP )) < 0 )
   {
      cout << "Cannot open socket!" << endl;
      return;
   }
   
   if( setsockopt( sock, SOL_SOCKET, SO_BROADCAST, (char *) &optval, sizeof( optval )) < 0 )
   {
      cout << "Cannot set socket options!" << endl;
      return;
   }
   
   struct sockaddr_in addr;
   unsigned char packet[17*6];
   int i, j;

   addr.sin_family = AF_INET;
   addr.sin_port   = htons(9);
   if( inet_aton( "255.255.255.255", &addr.sin_addr ) == 0 )
   {
      cout << "Invalid remote ip address given!" << endl;
      return;
   }

   for( i = 0; i < 6; i++ )
   {
      packet[i] = 0xFF;
   }

   for( i = 1; i <= 16; i++ )
   {
      for( j = 0; j < 6; j++ )
      {
         packet[i * 6 + j] = macAddr[j];
      }
   }
   
   for (int i = 0 ; i < 17; i++)
   {
      for (int j = 0; j < 6; j++)
      {
         cout << hex << setw(2) << setfill('0') << int(packet[i * 6 + j]) << " ";
      }
      cout << endl;
   }
   
   if( sendto( sock, packet, sizeof( packet ), 0, (struct sockaddr *) &addr, sizeof( addr )) < 0 )
   {
      cout << "Cannot send data!" << endl;
      return;
   }
}


void Ircc::test(void)
{
   cout << "Running IRCC test" << endl;
   
   wakeOnLan();

   usleep(20000000);
   
   getDial();
   getIrcc();
   getDmr();
//   registerNoAuth();
//
//   cout << "Enter pin number:" << endl;
//   
//   string auth = "";
//   
//   cin >> auth;
//   
//   auth = ":" + auth;
//   
//   string retStr = base64_encode(auth);
//   
//   cout << "Authstring: -" << retStr << "-" << endl;
//   
//   registerAuth(retStr);

   sendCode(irccCode_t::irccPlay);
   usleep(5000000);
   sendCode(irccCode_t::irccPause);
   usleep(5000000);
   sendCode(irccCode_t::irccPlay);

   cout << "Finished" << endl;
}


