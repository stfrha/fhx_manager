#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <iomanip>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <curl/curl.h>
#include "ircc.h"
#include "base64.h"

using namespace std;

// Sony Blu-ray player. Give it a DHCP reservation in the router so this
// address stays valid.
static const string bdIpAddress = "192.168.0.198";
static const string bdMacAddress = "64:ff:0a:d5:b2:60";

// MAC address of this Raspberry Pi, used as device id when registering
static const string ownMacAddress = "b8-27-eb-24-95-16";

static const string userAgent = "FHX Manager/0.1 (Raspberry Pi 3b+)";

// IRCC commands must never block the controller for long. If the player
// is off or unreachable we give up quickly.
static const long irccTimeoutMs = 2000;
static const long infoTimeoutMs = 5000;

static size_t discardResponse(char* ptr, size_t size, size_t nmemb, void* userdata)
{
   (void)ptr;
   (void)userdata;
   return size * nmemb;
}

static size_t storeResponse(char* ptr, size_t size, size_t nmemb, void* userdata)
{
   ((string*)userdata)->append(ptr, size * nmemb);
   return size * nmemb;
}

Ircc::Ircc()
{   
   // Must be called once before curl is used from any thread
   curl_global_init(CURL_GLOBAL_DEFAULT);


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

// Performs a GET (postBody == NULL) or POST request. Returns true if the
// player answered with HTTP 200.
bool Ircc::httpRequest(const string& url,
                       const vector<string>& headers,
                       const string* postBody,
                       long timeoutMs)
{
   CURL *curl = curl_easy_init();
   if (!curl)
   {
      cout << "IRCC: curl_easy_init() failed" << endl;
      return false;
   }

   struct curl_slist *list = NULL;
   for (const string& h : headers)
   {
      list = curl_slist_append(list, h.c_str());
   }
   list = curl_slist_append(list, "Connection: close");

   string response;

   curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
   curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
   curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent.c_str());
   curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip");
   curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, timeoutMs);
   curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeoutMs);
   // Required for timeouts to work safely in a multithreaded program
   curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

   if (postBody)
   {
      // Let curl compute Content-Length from the body
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postBody->c_str());
      curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)postBody->size());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discardResponse);
   }
   else
   {
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, storeResponse);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
   }

   bool ok = false;
   CURLcode res = curl_easy_perform(curl);
   if (res != CURLE_OK)
   {
      cout << "IRCC: request to " << url << " failed: " << curl_easy_strerror(res) << endl;
   }
   else
   {
      long httpCode = 0;
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
      if (httpCode == 200)
      {
         ok = true;
      }
      else
      {
         cout << "IRCC: " << url << " answered HTTP " << httpCode << endl;
      }
   }

   if (!response.empty())
   {
      cout << response << endl;
   }

   curl_slist_free_all(list);
   curl_easy_cleanup(curl);

   return ok;
}

void Ircc::getDial(void)
{
   cout << "Requesting Dial.xml..." << endl;
   httpRequest("http://" + bdIpAddress + ":50201/dial.xml", {}, NULL, infoTimeoutMs);
}

void Ircc::getIrcc(void)
{
   cout << "Requesting Ircc.xml..." << endl;
   httpRequest("http://" + bdIpAddress + ":50001/Ircc.xml", {}, NULL, infoTimeoutMs);
}

void Ircc::getDmr(void)
{
   cout << "Requesting Dmr.xml..." << endl;
   httpRequest("http://" + bdIpAddress + ":52323/dmr.xml",
               { "X-AV-Client-Info: av=5.0; cn=\"Fredrik Hoffman\"; mn=\"FHX Manager\"; mv=\"0.1.0\";" },
               NULL,
               infoTimeoutMs);
}

static string registerUrl(void)
{
   return "http://" + bdIpAddress + ":50002/register?name=BiorummetRPi+FHX_Manager"
          "&registrationType=initial&deviceId=FHX_Manager%3A" + ownMacAddress + "&wolSupport=true";
}

void Ircc::registerNoAuth(void)
{
   cout << "Registering without auth" << endl;

   httpRequest(registerUrl(),
               { "X-CERS-DEVICE_ID: FHX_Manager:" + ownMacAddress,
                 "X-CERS-DEVICE_INFO: Raspbian/FHX_Manager/BiorummetRPi" },
               NULL,
               infoTimeoutMs);
}

void Ircc::registerAuth(string authStr)
{
   cout << "Registering with auth" << endl;

   httpRequest(registerUrl(),
               { "X-CERS-DEVICE_ID: FHX_Manager:" + ownMacAddress,
                 "Authorization: Basic " + authStr,
                 "X-CERS-DEVICE_INFO: Raspbian/FHX_Manager/BiorummetRPi" },
               NULL,
               infoTimeoutMs);
}

void Ircc::sendCode(irccCode_t code)
{
   if ((code < 0) || (code >= irccLastIrccCode) || ((size_t)code >= m_irccCmnds.size()))
   {
      cout << "IRCC: invalid code " << (int)code << endl;
      return;
   }

   cout << "Send IRCC " << (int)code << endl;

   string url = "http://" + bdIpAddress + ":50001/upnp/control/IRCC";
   string post = "<?xml version=\"1.0\"?><s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><s:Body><u:X_SendIRCC xmlns:u=\"urn:schemas-sony-com:service:IRCC:1\"><IRCCCode>" + m_irccCmnds[(int)code] + "</IRCCCode></u:X_SendIRCC></s:Body></s:Envelope>";

   httpRequest(url,
               { "SOAPACTION: \"urn:schemas-sony-com:service:IRCC:1#X_SendIRCC\"",
                 "Content-Type: text/xml; charset=utf-8",
                 "X-CERS-DEVICE_ID: FHX_Manager:" + ownMacAddress },
               &post,
               irccTimeoutMs);
}

void Ircc::wakeOnLan(void)
{
   unsigned char macAddr[6];

   if (sscanf(bdMacAddress.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
              &macAddr[0], &macAddr[1], &macAddr[2],
              &macAddr[3], &macAddr[4], &macAddr[5]) != 6)
   {
      cout << "WoL: invalid MAC address " << bdMacAddress << endl;
      return;
   }

   // Magic packet: 6 x 0xFF followed by the MAC address 16 times
   unsigned char packet[17*6];
   memset(packet, 0xFF, 6);
   for (int i = 1; i <= 16; i++)
   {
      memcpy(&packet[i * 6], macAddr, 6);
   }

   int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
   if (sock < 0)
   {
      cout << "WoL: cannot open socket: " << strerror(errno) << endl;
      return;
   }

   int optval = 1;
   if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *) &optval, sizeof(optval)) < 0)
   {
      cout << "WoL: cannot set socket options: " << strerror(errno) << endl;
      close(sock);
      return;
   }

   // Send both to the limited broadcast and the subnet broadcast (some
   // routers/WiFi setups do not forward 255.255.255.255), on ports 9 and 7.
   const char* targets[] = { "255.255.255.255", "192.168.0.255" };
   const int ports[] = { 9, 7 };
   bool sent = false;

   for (const char* target : targets)
   {
      for (int port : ports)
      {
         struct sockaddr_in addr;
         memset(&addr, 0, sizeof(addr));
         addr.sin_family = AF_INET;
         addr.sin_port = htons(port);
         inet_aton(target, &addr.sin_addr);

         if (sendto(sock, packet, sizeof(packet), 0, (struct sockaddr *) &addr, sizeof(addr)) < 0)
         {
            cout << "WoL: cannot send to " << target << ":" << port << ": " << strerror(errno) << endl;
         }
         else
         {
            sent = true;
         }
      }
   }

   close(sock);

   if (sent)
   {
      cout << "WoL: magic packet sent to " << bdMacAddress << endl;
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

