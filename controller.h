#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <fstream>
#include <string>

#include "led_strip.h"
#include "Dali.h"

class Comms;

class Controller
{
public:
   enum ControllerState
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
   ControllerState m_state;
   ControllerState m_prevState;

   LedStrip m_ledStrip;
   Dali m_dali;
   Comms* m_comms;
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
   Controller(Comms* comms);
   
   void initializeController(void);

   void executeCommand(std::string command);
};

#endif