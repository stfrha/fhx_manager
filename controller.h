#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <fstream>
#include <string>

#include "led_strip.h"
#include "dali.h"
#include "benq.h"

class Comms;
class Ynca;

class Controller
{
public:
   enum LightControllerState
   {
      allOn,
      allOff,
      preMovie,
      movie,
      pause,
      endCredits,
      kidsMovie
   };
  
private:
   LightControllerState m_state;
   LightControllerState m_prevState;

   LedStrip m_ledStrip;
   Dali m_dali;
   Benq m_benq;
   Comms* m_comms;
   Ynca* m_ynca;
   bool m_yamahaOn;
   bool m_benqOn;
   bool m_lightOn;
   bool m_stateChangePending;
   bool m_ledOverridePending;
   unsigned int m_redColorOverride;
   unsigned int m_greenColorOverride;
   unsigned int m_blueColorOverride;


   void prepareStatusMessage(void);
   std::string generateStatusMessage(int precision);
       
   static void* monitorThread(void* threadId);

         
public:
   Controller(Comms* comms, Ynca* ynca);
   
   void initializeController(void);

   void executeCommand(std::string command);
};

#endif