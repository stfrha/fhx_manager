#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <fstream>
#include <string>

#include "led_strip.h"
#include "dali.h"
#include "benq.h"
#include "ir.h"
#include "ircc.h"
#include "script.h"
#include "commands.h"

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
   Ir m_ir;
   Ircc m_ircc;
   Script m_script;
   
   bool m_yamahaOn;
   bool m_benqOn;
   bool m_lightOn;
   bool m_stateChangePending;
   bool m_ledOverridePending;
   unsigned int m_redColorOverride;
   unsigned int m_greenColorOverride;
   unsigned int m_blueColorOverride;
   std::string m_startHour;
   std::string m_startMinute;
   std::string m_featureDuration;
   std::string m_endCredits;
   
   bool m_oneTimeIrInit;

   void setNewLightState(LightControllerState newState);
   void prepareStatusMessage(void);
   std::string generateStatusMessage(int precision);
       
   static void* lightControllerThread(void* threadId);

         
public:
   Controller(Comms* comms, Ynca* ynca);
   
   void initializeController(void);

   void setBlackScreen(void);
   std::string executeCommand(const CommandStruct& command);
};

#endif