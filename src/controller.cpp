#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sstream>
#include <iomanip>
#include <time.h>

// For threads
#include <pthread.h>

#include "controller.h"
#include "comms.h"
#include "ynca.h"


using namespace std;

pthread_mutex_t globalStatusMutex = PTHREAD_MUTEX_INITIALIZER;

string g_latestStatus;

Controller::Controller(Comms* comms, Ynca* ynca) :
   m_comms(comms),
   m_ynca(ynca),
   m_state(allOn),
   m_prevState(m_state),
   m_ir(),
   m_yamahaOn(false),
   m_benqOn(false),
   m_lightOn(true),
   m_stateChangePending(false),
   m_ledOverridePending(false),
   m_oneTimeIrInit(true)
{
   
}

void Controller::initializeController(void)
{
   cout << "Setting up HW..." << endl;
   
   if (wiringPiSetup() == -1)
   {
      fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
      return;
   }

   pthread_t thread;

   int result = pthread_create(&thread, NULL, monitorThread, this);
   if (result)
   {
      cout << "Monitor thread could not be created, " << result << endl;
      exit(1);
   }
   
   if (!m_benq.initializeBenq())
   {
      cout << "Error initializing Benq." << endl;
   }

   // Yamaha is ok to communicate with even before comms has been initialized
   m_yamahaOn = m_ynca->isYamahaOn();
   m_benqOn = m_benq.isBenqOn();
   
}

void* Controller::monitorThread(void* cntrlPointer)
{
   Controller* instance = (Controller*)cntrlPointer;

   if (!instance->m_ledStrip.initializeLedStrip())
   {
      cout << "Error initializing Led strip." << endl;
      while (true)
      {
      }
   }

   if (!instance->m_dali.initializeDali())
   {
      cout << "Error initializing Dali." << endl;
      while (true)
      {
      }
   }

   instance->m_lightOn = instance->m_dali.isLightsOn();
   if (instance->m_lightOn)
   {
      instance->m_prevState = instance->m_state;
      instance->m_state = allOn;
   }
   else
   {
      instance->m_prevState = instance->m_state;
      instance->m_state = allOff;
   }


   while (true)
   {
      if (instance->m_dali.isLightsOn())
      {
         if (!instance->m_lightOn)
         {
            instance->m_prevState = instance->m_state;
            instance->m_state = allOn;

            cout << "Led stripe on at random color" << endl;

            int colorCode = rand() % 7;
            switch (colorCode)
            {
            case 0:
               instance->m_ledStrip.fadeToColor(0x64, 0x64, 0x64, 4.5, EaseInQuad);
               break;
            case 1:
               instance->m_ledStrip.fadeToColor(0x64, 0x0, 0x0, 4.5, EaseInQuad);
               break;
            case 2:
               instance->m_ledStrip.fadeToColor(0x0, 0x64, 0x00, 4.5, EaseInQuad);
               break;
            case 3:
               instance->m_ledStrip.fadeToColor(0x0, 0x0, 0x64, 4.5, EaseInQuad);
               break;
            case 4:
               instance->m_ledStrip.fadeToColor(0x64, 0x64, 0x0, 4.5, EaseInQuad);
               break;
            case 5:
               instance->m_ledStrip.fadeToColor(0x0, 0x64, 0x64, 4.5, EaseInQuad);
               break;
            case 6:
               instance->m_ledStrip.fadeToColor(0x64, 0x0, 0x64, 4.5, EaseInQuad);
               break;
            }
         }
         instance->m_lightOn = true;

         if (instance->m_stateChangePending)
         {
            // Now execute the light settings for the current state
            switch (instance->m_state)
            {
            case allOn:
               instance->m_dali.setFadeTime(0, 1);
               instance->m_dali.setFadeTime(1, 4);
               instance->m_dali.broadcastLightPower(0xFE);
               instance->m_ledStrip.fadeToColor(0x64, 0x64, 0x64, 4.5, EaseInQuad);
               cout << "State is All On." << endl;
               break;
            case allOff:
               instance->m_dali.setFadeTime(0, 1);
               instance->m_dali.setFadeTime(1, 4);
               instance->m_dali.broadcastLightPower(0x00);
               instance->m_ledStrip.fadeToColor(0x0, 0x0, 0x0, 4.5, EaseInQuad);
               cout << "State is All Off." << endl;
               break;
            case preMovie:
               instance->m_dali.setFadeTime(0, 8);
               instance->m_dali.setFadeTime(1, 10);
               instance->m_dali.setLightPower(0, 0xc0);
               instance->m_dali.setLightPower(1, 0x80);
               instance->m_ledStrip.fadeToColor(0x64, 0x64, 0x0, 4.5, EaseInQuad);
               cout << "State is Pre Movie." << endl;
               break;
            case movie:
               if (instance->m_prevState == preMovie)
               {
                  instance->m_dali.setFadeTime(0, 10);
                  instance->m_dali.setFadeTime(1, 10);
                  instance->m_dali.broadcastLightPower(0x00);
                  instance->m_ledStrip.fadeToColor(0x0, 0x0, 0x0, 4.5, EaseInQuad);
                  cout << "State is Movie (from preMovie)." << endl;
               }
               else
               {
                  instance->m_dali.setFadeTime(0, 1);
                  instance->m_dali.setFadeTime(1, 4);
                  instance->m_dali.broadcastLightPower(0x00);
                  instance->m_ledStrip.fadeToColor(0x0, 0x0, 0x0, 4.5, EaseInQuad);
                  cout << "State is Movie (from preMovie)." << endl;
               }
               break;
            case kidsMovie:
               if (instance->m_prevState == preMovie)
               {
                  instance->m_dali.setFadeTime(0, 10);
                  instance->m_dali.setFadeTime(1, 10);
                  instance->m_dali.setLightPower(0, 0x80);
                  instance->m_dali.setLightPower(1, 0x00);
                  instance->m_ledStrip.fadeToColor(0x64, 0x64, 0x64, 4.5, EaseInQuad);
                  cout << "State is Movie (from preMovie)." << endl;
               }
               else
               {
                  instance->m_dali.setFadeTime(0, 1);
                  instance->m_dali.setFadeTime(1, 4);
                  instance->m_dali.setLightPower(0, 0x80);
                  instance->m_dali.setLightPower(1, 0x00);
                  instance->m_ledStrip.fadeToColor(0x64, 0x64, 0x64, 4.5, EaseInQuad);
                  cout << "State is Movie (from preMovie)." << endl;
               }
               break;
            case pause:
               instance->m_dali.setFadeTime(0, 1);
               instance->m_dali.setFadeTime(1, 4);
               instance->m_dali.setLightPower(0, 0xC0);
               instance->m_dali.setLightPower(1, 0x40);
               instance->m_ledStrip.fadeToColor(0x0, 0x64, 0x64, 4.5, EaseInQuad);
               cout << "State is Pause." << endl;
               break;
            case endCredits:
               instance->m_dali.setFadeTime(0, 10);
               instance->m_dali.setFadeTime(1, 12);
               instance->m_dali.setLightPower(0, 0xC0);
               instance->m_dali.setLightPower(1, 0x80);
               instance->m_ledStrip.fadeToColor(0x0, 0x0, 0x64, 4.5, EaseInQuad);
               cout << "State is End credits." << endl;
               break;

            }

            instance->m_stateChangePending = false;

         }
         else if (instance->m_ledOverridePending)
         {
            instance->m_ledStrip.setColor(instance->m_redColorOverride, instance->m_greenColorOverride, instance->m_blueColorOverride);            
            instance->m_ledOverridePending = false;
         }
      }
      else
      {
         if (instance->m_lightOn)
         {
            cout << "Led stripe off" << endl;
            instance->m_ledStrip.fadeToColor(0, 4.5, EaseInQuad);
         }
         else if (instance->m_ledOverridePending)
         {
            instance->m_ledStrip.setColor(instance->m_redColorOverride, instance->m_greenColorOverride, instance->m_blueColorOverride);            
            instance->m_ledOverridePending = false;
         }

         // cout << "Setting m_lightOn to false" << endl;
         instance->m_lightOn = false;

      }
      delay(250);
   }
}

/* Messages are 16 bytes:

ALON------------  - All on
ALOF------------  - All off
PRMV------------  - Pre Movie
MOVI------------  - Movie
KDMO------------  - Kids Movie (some light, but not on screen)
PAUS------------  - Pause
ENCR------------  - End Credits
SREQ------------  - Request status string
SYSTEMOFF-------  - Turn system off
VOLUMEUP--------  - Volume up
VOLUMEDOWN------  - Volume down
VOLUMEVAL(-45.5)  - Volume down Could also be VOLUMEVAL(2)----
SOURCEPS--------  - Start source PS
SOURCECC--------  - Start source CC
SOURCETV--------  - Start source TV
LCC(128,128,128)  - Led color command
DH660POWER------  - Dilog IR Code 
DH660MUTE-------  - Dilog IR Code 
DH660LIST-------  - Dilog IR Code 
DH660TIME-------  - Dilog IR Code 
DH660K1---------  - Dilog IR Code 
DH660K2---------  - Dilog IR Code 
DH660K3---------  - Dilog IR Code 
DH660K4---------  - Dilog IR Code 
DH660K5---------  - Dilog IR Code 
DH660K6---------  - Dilog IR Code 
DH660K7---------  - Dilog IR Code 
DH660K8---------  - Dilog IR Code 
DH660K9---------  - Dilog IR Code 
DH660K0---------  - Dilog IR Code 
DH660CHANUP-----  - Dilog IR Code 
DH660CHANDWN----  - Dilog IR Code 
DH660INFO-------  - Dilog IR Code 
DH660DIRECTORY--  - Dilog IR Code 
DH660MENU-------  - Dilog IR Code 
DH660EPG--------  - Dilog IR Code 
DH660UP---------  - Dilog IR Code 
DH660EXIT-------  - Dilog IR Code 
DH660LEFT-------  - Dilog IR Code 
DH660SELECT-----  - Dilog IR Code 
DH660RIGHT------  - Dilog IR Code 
DH660SUBTITLE---  - Dilog IR Code 
DH660DOWN-------  - Dilog IR Code 
DH660TEXT-------  - Dilog IR Code 
DH660STOP-------  - Dilog IR Code 
DH660PLAY-------  - Dilog IR Code 
DH660PAUSE------  - Dilog IR Code 
DH660RECORD-----  - Dilog IR Code 
DH660RED--------  - Dilog IR Code 
DH660GREEN------  - Dilog IR Code 
DH660YELLOW-----  - Dilog IR Code 
DH660BLUE-------  - Dilog IR Code 
DH660NEXT-------  - Dilog IR Code 
DH660PREVIOUS---  - Dilog IR Code 
DH660AUDIO------  - Dilog IR Code 
DH660ZOOM-------  - Dilog IR Code 
*/

// The following method will be called by and executed in
// another thread than the main
// This method is the only place (after initialization)
// where the m_state member and the relays are set. Therefore
// there is no need to mutex protect them. 

void Controller::executeCommand(std::string command)
{
   if (m_oneTimeIrInit)
   {
      m_oneTimeIrInit = false;
      
      m_ir.initialize();
   }
   
   cout << "Received command: " << command << endl;
   
   if (command == "ALON------------")
   {
      m_prevState = m_state;
      m_state = allOn;
      m_stateChangePending = true;
   
      cout << "State is All On." << endl;
   }
   else if (command == "ALOF------------")
   {
      m_prevState = m_state;
      m_state = allOff;
      m_stateChangePending = true;

      cout << "State is All Off." << endl;
   }
   else if (command == "PRMV------------")
   {
      m_prevState = m_state;
      m_state = preMovie;
      m_stateChangePending = true;

      cout << "State is Pre Movie." << endl;
   }
   else if (command == "MOVI------------")
   {
      m_prevState = m_state;
      m_state = movie;
      m_stateChangePending = true;

      cout << "State is Movie." << endl;
   }
   else if (command == "KDMO------------")
   {
      m_prevState = m_state;
      m_state = kidsMovie;
      m_stateChangePending = true;

      cout << "State is Kids Movie." << endl;
   }
   else if (command == "PAUS------------")
   {
      m_prevState = m_state;
      m_state = pause;
      m_stateChangePending = true;

      cout << "State is Pause." << endl;
   }
   else if (command == "ENCR------------")
   {
      m_prevState = m_state;
      m_state = endCredits;
      m_stateChangePending = true;

      cout << "State is End credits." << endl;
   }
   else if (command == "TESTYAMAHACOMM--")
   {
      cout << "Command for Yamaha comm test." << endl;
      m_comms->yamahaClientComm();      
      cout << "Command for Yamaha comm test finished." << endl;
   }
   else if (command == "SYSTEMOFF-------")
   {
      m_ynca->turnOff();
      m_benq.turnOff();
      m_yamahaOn = false;
      m_benqOn = false;
   }
   else if (command == "VOLUMEUP--------")
   {
      m_ynca->volUp();
   }
   else if (command == "VOLUMEDOWN------")
   {
      m_ynca->volDown();
   }
   else if (command == "SOURCEPS--------")
   {
      m_benq.turnOn();
      m_ynca->startSource(playStation);
      m_yamahaOn = true;
      m_benqOn = true;

   }
   else if (command == "SOURCECC--------")
   {
      m_benq.turnOn();
      m_ynca->startSource(chromecast);
      m_yamahaOn = true;
      m_benqOn = true;
   }
   else if (command == "SOURCETV--------")
   {
      m_benq.turnOn();
      m_ynca->startSource(television);
      m_yamahaOn = true;
      m_benqOn = true;
   }
   else if (command == "SOURCESPOTIFY---")
   {
      m_ynca->startSource(spotify);
      m_yamahaOn = true;
   }
   else if (command == "SOURCEVINYL-----")
   {
      m_ynca->startSource(vinyl);
      m_yamahaOn = true;
   }
   else if (command == "SOURCETUNER-----")
   {
      m_ynca->startSource(tuner);
      m_yamahaOn = true;
   }
   else if (command == "SOURCERPI-------")
   {
      m_benq.turnOn();
      m_ynca->startSource(raspberryPi);
      m_yamahaOn = true;
      m_benqOn = true;
   }
   else if (command == "SOURCEAUX-------")
   {
      m_ynca->startSource(aux);
      m_yamahaOn = true;
   }
   else if (command == "SOURCEBR--------")
   {
      m_benq.turnOn();
      m_ynca->startSource(bluRay);
      m_yamahaOn = true;
      m_benqOn = true;
   }
   else if (command.substr(0, 9) == "VOLUMEVAL")
   {
      std::string volValStr = command.substr(10);
      volValStr = volValStr.substr(0, volValStr.find(")"));
      
      // Check so that first decimal place is there
      if (volValStr.find(".") == string::npos)
      {
         volValStr += ".0";
      }
      
      cout << "Setting volume: " << volValStr << endl;
      
      m_ynca->setVolume(volValStr);
   }
   else if (command.substr(0, 5) == "DH660")
   {
      cout << "Found DILOG DH660-HD IR Command" << endl;
      
      if (command.substr(5) == "POWER------")
      {
         cout << "Found DILOG DH660-HD IR Command 'KEY_POWER'" << endl;
         m_ir.sendIr("dh_660hd", "KEY_POWER");
      }
   }
   else
   {
      // Check if it is LCC
      std::string stumpCommand = command.substr(0, 3);
      
      if (stumpCommand == "LCC")
      {
         // Decode color values and scale them to max
         // being 100. pwm = (color * 100) / 255
         
         // Command is LCC(128,128,128)

         std::string redStr = command.substr(4, 3);
         std::string greenStr = command.substr(8, 3);
         std::string blueStr = command.substr(12, 3);
     
         m_redColorOverride = (stoi(redStr) * 100) / 255;
         m_greenColorOverride = (stoi(greenStr) * 100) / 255;
         m_blueColorOverride = (stoi(blueStr) * 100) / 255;
         
         m_ledOverridePending = true;        
      }
      else
      {
         cout << "In future, will execute command: " << command << endl;
      }
   }

   prepareStatusMessage();

}

string Controller::generateStatusMessage(int precision)
{
   // Latest status is on the form:
   // {Light power on=1 or off=2};{Light state};{Yamaha power, 1=On, 2=Standby};{Benq power, 1=On, 2=Off};{Volume -23.5}
   // Example: "2;4;2;2;-23.5" Light power on, Movie state, Yamaha on, Benq On, Volume -23.5 dB

   ostringstream statStream;

   if (m_lightOn)
   {
      statStream << "1;";
   }
   else
   {
      statStream << "2;";
   }
   switch (m_state)
   {
   case allOn:
      statStream << "1;";
      break;
   case allOff:
      statStream << "2;";
      break;
   case preMovie:
      statStream << "3;";
      break;
   case movie:
      statStream << "4;";
      break;
   case pause:
      statStream << "5;";
      break;
   case endCredits:
      statStream << "6;";
      break;
   case kidsMovie:
      statStream << "7;";
      break;
   }
   if (m_yamahaOn)
   {
      statStream << "1;";
   }
   else
   {
      statStream << "2;";
   }
   if (m_benqOn)
   {
      statStream << "1;";
   }
   else
   {
      statStream << "2;";
   }
   
   statStream << m_ynca->getVolume() << ";";
   
   statStream << (int)m_ynca->getCurrentSource();

   return statStream.str();
}


void Controller::prepareStatusMessage(void)
{
   string stat = generateStatusMessage(3);

   // This method is called from multiple threads
   // The global status needs thus be protected
   // by mutex.
   pthread_mutex_lock(&globalStatusMutex);
   g_latestStatus = stat;
   pthread_mutex_unlock(&globalStatusMutex);
}
