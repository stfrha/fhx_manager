#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sstream>
#include <iomanip>
#include <time.h>
#include <unistd.h>
#include <experimental/filesystem>
#include <vector>

// For threads
#include <pthread.h>

// For mutex
#include <mutex>

// For display manipulation
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>


#include "controller.h"
#include "comms.h"
#include "ynca.h"

using namespace std;

extern t_scriptStates scriptState;
extern string scriptFileName;

extern t_theatreStates theatreState;
extern string theatreFileName;

mutex controllerMutex;

//pthread_mutex_t globalStatusMutex = PTHREAD_MUTEX_INITIALIZER;

//string g_latestStatus;

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

   int result = pthread_create(&thread, NULL, lightControllerThread, this);
   if (result)
   {
      cout << "Light Control thread could not be created, " << result << endl;
      exit(1);
   }
   
   m_script.initializeScript(this);
   
   if (!m_benq.initializeBenq())
   {
      cout << "Error initializing Benq." << endl;
   }

   // Yamaha is ok to communicate with even before comms has been initialized
   m_yamahaOn = m_ynca->isYamahaOn();
   m_benqOn = m_benq.isBenqOn();
   
}


// This thread holds control over DALI and led strip (the only control?)
// It is managed by the variable m_state which it both monitors (can be
// set outside the thread) and writes to. There is a race condition here!
// It is also affected by the state of the dali controller which can be 
// on or off (light switch on wall)
// The Dali and led stripe is controlled using the controller instance
// which is passed to the thread. The Controller class owns these objects
// privately and does not provide any interface for outside control.
// Inside the controller, the led strip and dali is only communicated to from the 
// thread. Perhaps the thread shall own these objects on its own.
// Controll of the thread is done using the following member variables of the 
// controller instance:

// Member:                 Accessed outside thread:
// m_state                 rw
// m_prevState             rw
// m_stateChangePending    rw (set to true outside thread, set to false inside thread)
// m_lightOn               r  (to generate status)
// 
// 

void* Controller::lightControllerThread(void* cntrlPointer)
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

   controllerMutex.lock();

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

   controllerMutex.unlock();
   
   //int clock = 0;

   while (true)
   {
      //cout << "light control thread count: " << clock << endl;
      if (instance->m_dali.isLightsOn())
      {
         if (!instance->m_lightOn)
         {
            controllerMutex.lock();

            instance->m_prevState = instance->m_state;
            instance->m_state = allOn;

            controllerMutex.unlock();

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
            cout << "Handled a light state change" << endl;

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
      usleep(250000);
      // clock++;
   }
}

// TODO: The following method can be called from different
// threads: 
// 1. Communication server (socket)
// 2. Script execution
// All resources controlled by it must be mutex protected.

void Controller::setNewLightState(LightControllerState newState)
{
   cout << "Getting mutex for state change..." << endl;
   controllerMutex.lock();
   cout << "Mutex locked" << endl;

   m_prevState = m_state;
   m_state = newState;
   m_stateChangePending = true;

   cout << "Releasing mutex for state change" << endl;
   controllerMutex.unlock();
   cout << "Mutex released" << endl;
}

std::string Controller::executeCommand(const CommandStruct& command)
// Returns with status message
{
   
   if (m_oneTimeIrInit)
   {
      m_oneTimeIrInit = false;
      
      m_ir.initialize();
   }
   
   cout << "Received command - id: " << command.m_id << ", argument: " << command.m_argument << endl;

   m_script.recordCommand(command);
   
   if (command.m_id == t_commands::lightAllOn)
   {
      setNewLightState(allOn);

      // m_script.startScript("/home/pi/fhx_manager/scripts/test_script");
   
      cout << "State is All On." << endl;
   }
   else if (command.m_id == t_commands::lightAllOff)
   {
      setNewLightState(allOff);

      cout << "State is All Off." << endl;
   }
   else if (command.m_id == t_commands::lightPreMovie)
   {
      setNewLightState(preMovie);

      cout << "State is Pre Movie." << endl;
   }
   else if (command.m_id == t_commands::lightMovie)
   {
      setNewLightState(movie);

      cout << "State is Movie." << endl;
   }
   else if (command.m_id == t_commands::lightKidsMovie)
   {
      setNewLightState(kidsMovie);

      cout << "State is Kids Movie." << endl;
   }
   else if (command.m_id == t_commands::lightPause)
   {
      setNewLightState(pause);

      cout << "State is Pause." << endl;
   }
   else if (command.m_id == t_commands::lightEndCred)
   {
      setNewLightState(endCredits);

      cout << "State is End credits." << endl;
   }
   // else if (command.m_id == t_commands::lightAllOn)
   // {
      // cout << "Command for Yamaha comm test." << endl;
      // m_comms->yamahaClientComm();      
      // cout << "Command for Yamaha comm test finished." << endl;
   // }
   else if (command.m_id == t_commands::ledStrip)
   {
      // Decode color values and scale them to max
      // being 100. pwm = (color * 100) / 255
      // argument is (128,128,128)

      std::string redStr = command.m_argument.substr(1, 3);
      std::string greenStr = command.m_argument.substr(5, 3);
      std::string blueStr = command.m_argument.substr(9, 3);
  
      m_redColorOverride = (stoi(redStr) * 100) / 255;
      m_greenColorOverride = (stoi(greenStr) * 100) / 255;
      m_blueColorOverride = (stoi(blueStr) * 100) / 255;
      
      m_ledOverridePending = true;        
     
   }
   else if (command.m_id == t_commands::system_off)
   {
      m_ynca->turnOff();
      m_benq.turnOff();
      m_yamahaOn = false;
      m_benqOn = false;
   }
   else if (command.m_id == t_commands::volUp)
   {
      m_ynca->volUp();
   }
   else if (command.m_id == t_commands::volDown)
   {
      m_ynca->volDown();
   }
   else if (command.m_id == t_commands::volSet)
   {
      std::string volValStr = command.m_argument;
      volValStr = volValStr.substr(0, volValStr.find(")"));
      
      // Check so that first decimal place is there
      if (volValStr.find(".") == string::npos)
      {
         volValStr += ".0";
      }
      
      cout << "Setting volume: " << volValStr << endl;
      
      m_ynca->setVolume(volValStr);
   }
   else if (command.m_id == t_commands::sourcePs)
   {
      m_benq.turnOn();
      m_ynca->startSource(playStation);
      m_yamahaOn = true;
      m_benqOn = true;

   }
   else if (command.m_id == t_commands::sourceCc)
   {
      m_benq.turnOn();
      m_ynca->startSource(chromecast);
      m_yamahaOn = true;
      m_benqOn = true;
   }
   else if (command.m_id == t_commands::sourceTv)
   {
      m_benq.turnOn();
      m_ynca->startSource(television);
      m_yamahaOn = true;
      m_benqOn = true;
   }
   else if (command.m_id == t_commands::sourceSpotify)
   {
      m_ynca->startSource(spotify);
      m_yamahaOn = true;
   }
   else if (command.m_id == t_commands::sourceVinyl)
   {
      m_ynca->startSource(vinyl);
      m_yamahaOn = true;
   }
   else if (command.m_id == t_commands::sourceTuner)
   {
      m_ynca->startSource(tuner);
      m_yamahaOn = true;
   }
   else if (command.m_id == t_commands::sourceRpi)
   {
      m_benq.turnOn();
      m_ynca->startSource(raspberryPi);
      m_yamahaOn = true;
      m_benqOn = true;
   }
   else if (command.m_id == t_commands::sourceAux)
   {
      m_ynca->startSource(aux);
      m_yamahaOn = true;
   }
   else if (command.m_id == t_commands::sourceBr)
   {
      cout << "Waking up Blu-ray player..." << endl;
      m_ircc.wakeOnLan();
      m_benq.turnOn();
      m_ynca->startSource(bluRay);
      m_yamahaOn = true;
      m_benqOn = true;
   }
   else if (command.m_id == t_commands::dilogPower) m_ir.sendIr("dh_660hd", "KEY_POWER");
   else if (command.m_id == t_commands::dilogMute) m_ir.sendIr("dh_660hd", "KEY_MUTE");
   else if (command.m_id == t_commands::dilogList) m_ir.sendIr("dh_660hd", "KEY_LIST");
   else if (command.m_id == t_commands::dilogTime) m_ir.sendIr("dh_660hd", "KEY_TIME");
   else if (command.m_id == t_commands::dilog1) m_ir.sendIr("dh_660hd", "KEY_1");
   else if (command.m_id == t_commands::dilog2) m_ir.sendIr("dh_660hd", "KEY_2");
   else if (command.m_id == t_commands::dilog3) m_ir.sendIr("dh_660hd", "KEY_3");
   else if (command.m_id == t_commands::dilog4) m_ir.sendIr("dh_660hd", "KEY_4");
   else if (command.m_id == t_commands::dilog5) m_ir.sendIr("dh_660hd", "KEY_5");
   else if (command.m_id == t_commands::dilog6) m_ir.sendIr("dh_660hd", "KEY_6");
   else if (command.m_id == t_commands::dilog7) m_ir.sendIr("dh_660hd", "KEY_7");
   else if (command.m_id == t_commands::dilog8) m_ir.sendIr("dh_660hd", "KEY_8");
   else if (command.m_id == t_commands::dilog9) m_ir.sendIr("dh_660hd", "KEY_9");
   else if (command.m_id == t_commands::dilog0) m_ir.sendIr("dh_660hd", "KEY_0");
   //else if (command.m_id == t_commands::dilog1) m_ir.sendIr("dh_660hd", "KEY_CHANNELUP");
   //else if (command.m_id == t_commands::dilogPower) m_ir.sendIr("dh_660hd", "KEY_CHANNELDOWN");
   else if (command.m_id == t_commands::dilogInfo) m_ir.sendIr("dh_660hd", "KEY_INFO");
   //else if (command.m_id == t_commands::dilogPower) m_ir.sendIr("dh_660hd", "KEY_DIRECTORY");
   else if (command.m_id == t_commands::dilogMenu) m_ir.sendIr("dh_660hd", "KEY_MENU");
   //else if (command.m_id == t_commands::dilogPower) m_ir.sendIr("dh_660hd", "KEY_VOLUMEDOWN");
   else if (command.m_id == t_commands::dilogUp) m_ir.sendIr("dh_660hd", "KEY_UP");
   else if (command.m_id == t_commands::dilogExit) m_ir.sendIr("dh_660hd", "KEY_EXIT");
   else if (command.m_id == t_commands::dilogLeft) m_ir.sendIr("dh_660hd", "KEY_LEFT");
   else if (command.m_id == t_commands::dilogOk) m_ir.sendIr("dh_660hd", "KEY_SELECT");
   else if (command.m_id == t_commands::dilogRight) m_ir.sendIr("dh_660hd", "KEY_RIGHT");
   else if (command.m_id == t_commands::dilogSubtitle) m_ir.sendIr("dh_660hd", "KEY_SUBTITLE");
   else if (command.m_id == t_commands::dilogDown) m_ir.sendIr("dh_660hd", "KEY_DOWN");
   else if (command.m_id == t_commands::dilogText) m_ir.sendIr("dh_660hd", "KEY_TEXT");
   else if (command.m_id == t_commands::dilogStop) m_ir.sendIr("dh_660hd", "KEY_STOP");
   else if (command.m_id == t_commands::dilogPlay) m_ir.sendIr("dh_660hd", "KEY_PLAY");
   else if (command.m_id == t_commands::dilogPause) m_ir.sendIr("dh_660hd", "KEY_PAUSE");
   else if (command.m_id == t_commands::dilogRecord) m_ir.sendIr("dh_660hd", "KEY_RECORD");
   else if (command.m_id == t_commands::dilogRed) m_ir.sendIr("dh_660hd", "KEY_RED");
   else if (command.m_id == t_commands::dilogGreen) m_ir.sendIr("dh_660hd", "KEY_GREEN");
   else if (command.m_id == t_commands::dilogYellow) m_ir.sendIr("dh_660hd", "KEY_YELLOW");
   else if (command.m_id == t_commands::dilogBlue) m_ir.sendIr("dh_660hd", "KEY_BLUE");
   else if (command.m_id == t_commands::dilogNext) m_ir.sendIr("dh_660hd", "KEY_NEXT");
   else if (command.m_id == t_commands::dilogPrevious) m_ir.sendIr("dh_660hd", "KEY_PREVIOUS");
   //else if (command.m_id == t_commands::dilogPower) m_ir.sendIr("dh_660hd", "KEY_AUDIO");
   //else if (command.m_id == t_commands::dilogPower) m_ir.sendIr("dh_660hd", "KEY_ZOOM");
   else if (command.m_id == t_commands::dilogEpg) m_ir.sendIr("dh_660hd", "KEY_EPG");
   
   else if (command.m_id == t_commands::bdOk) m_ircc.sendCode(irccCode_t::irccConfirm);
   else if (command.m_id == t_commands::bdUp) m_ircc.sendCode(irccCode_t::irccUp);
   else if (command.m_id == t_commands::bdDown) m_ircc.sendCode(irccCode_t::irccDown);
   else if (command.m_id == t_commands::bdRight) m_ircc.sendCode(irccCode_t::irccRight);
   else if (command.m_id == t_commands::bdLeft) m_ircc.sendCode(irccCode_t::irccLeft);
   // else if (command.m_id == t_commands::bdPower) m_ircc.sendCode(irccCode_t::irccHome);
   else if (command.m_id == t_commands::bdOptions) m_ircc.sendCode(irccCode_t::irccOptions);
   else if (command.m_id == t_commands::bdBack) m_ircc.sendCode(irccCode_t::irccReturn);
   //else if (command.m_id == t_commands::bdPower) m_ircc.sendCode(irccCode_t::irccNum1);
   //else if (command.m_id == t_commands::bdPower) m_ircc.sendCode(irccCode_t::irccNum2);
   //else if (command.m_id == t_commands::bdPower) m_ircc.sendCode(irccCode_t::irccNum3);
   //else if (command.m_id == t_commands::bdPower) m_ircc.sendCode(irccCode_t::irccNum4);
   //else if (command.m_id == t_commands::bdPower) m_ircc.sendCode(irccCode_t::irccNum5);
   //else if (command.m_id == t_commands::bdPower) m_ircc.sendCode(irccCode_t::irccNum6);
   //else if (command.m_id == t_commands::bdPower) m_ircc.sendCode(irccCode_t::irccNum7);
   //else if (command.m_id == t_commands::bdPower) m_ircc.sendCode(irccCode_t::irccNum8);
   //else if (command.m_id == t_commands::bdPower) m_ircc.sendCode(irccCode_t::irccNum9);
   //else if (command.m_id == t_commands::bdPower) m_ircc.sendCode(irccCode_t::irccNum0);
   else if (command.m_id == t_commands::bdPower) m_ircc.sendCode(irccCode_t::irccPower);
   // else if (command.m_id == t_commands::bdPower) m_ircc.sendCode(irccCode_t::irccDisplay);
   // else if (command.m_id == t_commands::bdPower) m_ircc.sendCode(irccCode_t::irccAudio);
   else if (command.m_id == t_commands::bdSubtitle) m_ircc.sendCode(irccCode_t::irccSubTitle);
   //else if (command.m_id == t_commands::bdPower) m_ircc.sendCode(irccCode_t::irccFavorites);
   else if (command.m_id == t_commands::bdYellow) m_ircc.sendCode(irccCode_t::irccYellow);
   else if (command.m_id == t_commands::bdBlue) m_ircc.sendCode(irccCode_t::irccBlue);
   else if (command.m_id == t_commands::bdRed) m_ircc.sendCode(irccCode_t::irccRed);
   else if (command.m_id == t_commands::bdGreen) m_ircc.sendCode(irccCode_t::irccGreen);
   else if (command.m_id == t_commands::bdPlay) m_ircc.sendCode(irccCode_t::irccPlay);
   else if (command.m_id == t_commands::bdStop) m_ircc.sendCode(irccCode_t::irccStop);
   else if (command.m_id == t_commands::bdPause) m_ircc.sendCode(irccCode_t::irccPause);
   else if (command.m_id == t_commands::bdRewind) m_ircc.sendCode(irccCode_t::irccRewind);
   else if (command.m_id == t_commands::bdFf) m_ircc.sendCode(irccCode_t::irccForward);
   else if (command.m_id == t_commands::bdPrevious) m_ircc.sendCode(irccCode_t::irccPrev);
   else if (command.m_id == t_commands::bdNext) m_ircc.sendCode(irccCode_t::irccNext);
   //else if (command.m_id == t_commands::bdPower) m_ircc.sendCode(irccCode_t::irccReplay);
   //else if (command.m_id == t_commands::bdPower) m_ircc.sendCode(irccCode_t::irccAdvance);
   //else if (command.m_id == t_commands::bdPower) m_ircc.sendCode(irccCode_t::irccAngle);
   else if (command.m_id == t_commands::bdTopMenu) m_ircc.sendCode(irccCode_t::irccTopMenu);
   else if (command.m_id == t_commands::bdPopupMenu) m_ircc.sendCode(irccCode_t::irccPopUpMenu);
   else if (command.m_id == t_commands::bdEject) m_ircc.sendCode(irccCode_t::irccEject);
   //else if (command.m_id == t_commands::bdPower) m_ircc.sendCode(irccCode_t::irccKaraoke);
   //else if (command.m_id == t_commands::bdPower) m_ircc.sendCode(irccCode_t::irccNetflix);
   //else if (command.m_id == t_commands::bdPower) m_ircc.sendCode(irccCode_t::irccMode3D);
   else if (command.m_id == t_commands::bdPowerOnLan) 
   {
      cout << "Waking up Blu-ray player..." << endl;
      m_ircc.wakeOnLan();
   }
   else if (command.m_id == t_commands::playScript)
   {
      m_script.startScript(command.m_argument);
   }
   else if (command.m_id == t_commands::pauseScript)
   {
      m_script.pauseScript();
   }
   else if (command.m_id == t_commands::stopPlayScript)
   {
      m_script.stopScript();
   }
   else if (command.m_id == t_commands::recScript)
   {
      // Extract feature duration, end credits duration and script file name
      cout << "Start feature command, argument: " << command.m_argument << endl;
      int delim1 = command.m_argument.find_first_of(';');
      int delim2 = command.m_argument.find_last_of(';');
      string file = command.m_argument.substr(0, delim1);
      
      m_featureDuration = command.m_argument.substr(delim1 + 1, delim2 - delim1 - 1 );
      m_endCredits = command.m_argument.substr(delim2 + 1);
      
      m_script.beginRecordScript(file, m_featureDuration, m_endCredits);
   }
   else if (command.m_id == t_commands::stopRecScript)
   {
      m_script.stopRecordScript();
   }
   else if (command.m_id == t_commands::continueScript)
   {
      m_script.continueScript();
   }
   else if (command.m_id == t_commands::startTheatreProgram)
   {
      // Extract start time and script file name
      cout << "Start feature command, argument: " << command.m_argument << endl;
      int delim1 = command.m_argument.find_first_of(';');
      int delim2 = command.m_argument.find_last_of(';');
      string file = command.m_argument.substr(0, delim1);
      
      m_startHour = command.m_argument.substr(delim1 + 1, delim2 - delim1 - 1 );
      m_startMinute = command.m_argument.substr(delim2 + 1);
      
//      using days = chrono::duration<int, std::ratio<86400>>;
//      chrono::time_point<chrono::system_clock, days> lastMidnight = chrono::time_point_cast<days>(chrono::system_clock::now());     
//      chrono::time_point<chrono::system_clock> lastMidnight2 = chrono::time_point_cast<days, std::chrono::system_clock>(lastMidnight);

      auto current = chrono::system_clock::now();
      auto now_timet = chrono::system_clock::to_time_t(current);
      auto now_local = localtime(&now_timet);
      cout << "Local Time " << put_time(now_local, "%c") << endl;
      auto midnight_local = now_local;
      midnight_local->tm_hour = 0; 
      midnight_local->tm_min = 0; 
      midnight_local->tm_sec = 0;
      auto midnight_timet = mktime(midnight_local);
      auto lastMidnight2 = chrono::system_clock::from_time_t(midnight_timet);
  
      double h = stod(m_startHour);
      double m = stod(m_startMinute);
      chrono::duration<double> dur(h * 3600.0 + m * 60.0);
      

    std::time_t now_c = std::chrono::system_clock::to_time_t(lastMidnight2);
    std::cout << "The time was just "
              << std::put_time(std::localtime(&now_c), "%F %T") << '\n';


      chrono::duration<double> featureWait = dur - (chrono::system_clock::now() - lastMidnight2);
      
      cout << "Will wait " << featureWait.count() << " for start of show" << endl;
      
      // Short wait, to use for debugging
      //featureWait = 5 * 1000ms;
   
      m_script.startTheatreProgram(file, featureWait);
   }
   else if (command.m_id == t_commands::abortTheatreProgram)
   {
      m_script.abortTheatreProgram();
   }
   else if (command.m_id == t_commands::pauseTheatreProgram)
   {
      m_script.pauseTheatreProgram();
   }
   else if (command.m_id == t_commands::continueTheatreProgram)
   {
      m_script.continueTheatreProgram();
   }

   return generateStatusMessage(3);
   
   
}

string Controller::generateStatusMessage(int precision)
{
   // Latest status is on the form:
   // {Light power on=1 or off=2};{Light state};{Yamaha power, 1=On, 2=Standby};{Benq power, 1=On, 2=Off}; \
      {Volume -23.5};{script state};{active script name};{theatre state};{theatre feature name};\
{start hours};{start minute}
   // Example: "2;4;2;2;-23.5" Light power on, Movie state, Yamaha on, Benq On, Volume -23.5 dB
   // Script states: 1=idle, 2=recording, 3=playing, 4=paused

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
   
   statStream << (int)m_ynca->getCurrentSource() << ";";
   
   statStream << ((int)scriptState + 1) << ";";
   
   statStream << scriptFileName << ";";

   statStream << ((int)theatreState + 1) << ";";
   
   statStream << theatreFileName << ";";

   statStream << m_startHour << ";";

   statStream << m_startMinute << ";";

   statStream << m_featureDuration << ";";

   statStream << m_endCredits;

   return statStream.str();
}



void Controller::setBlackScreen(void)
{
   int fbfd = 0;
   struct fb_var_screeninfo vinfo;
   struct fb_fix_screeninfo finfo;
   long int screensize = 0;
   char *fbp = 0;

   // Open the file for reading and writing
   fbfd = open("/dev/fb0", O_RDWR);
   if (!fbfd) 
   {
      cout << "Error: cannot open framebuffer device." << endl;
      return;
   }
   cout << "The framebuffer device was opened successfully." << endl;

   // Get fixed screen information
   if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) 
   {
      cout << "Error reading fixed information." << endl;
   }

   // Get variable screen information
   if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) 
   {
      cout << "Error reading variable information." << endl;
   }
   cout << vinfo.xres << "x" << vinfo.yres << ", " << vinfo.bits_per_pixel << " bpp" << endl;

   // map framebuffer to user memory 
   screensize = finfo.smem_len;

   fbp = (char*)mmap(0, 
      screensize, 
      PROT_READ | PROT_WRITE, 
      MAP_SHARED, 
      fbfd, 0);

   if ((int)fbp == -1) 
   {
      cout << "Failed to mmap." << endl;
   }
   else 
   {
      // draw...
      memset(fbp, 0x00, screensize);
   }
   
//   // cleanup
//   munmap(fbp, screensize);
//   close(fbfd);


}
