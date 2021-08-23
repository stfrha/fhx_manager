#ifndef SCRIPT_H
#define SCRIPT_H

#include <chrono>

#include "commands.h"

class Controller;

enum t_scriptStates
{
   scriptIdle,
   recording,
   playing,
   paused
};

enum t_theatreStates
{
   theatreIdle,
   theatreArmed,
   theatreRunning,
   featureRunning,
   featurePaused,
   theatreEnd
};


class ScheduledCommand
{
public:
   std::chrono::duration<double> m_time;
   CommandStruct m_command;
   
   ScheduledCommand(std::chrono::duration<double> time, const CommandStruct& command) :
      m_time(time),
      m_command(command)
   {
      
   }
};

class Script
{
private:
   Controller* m_controller;
   
   std::chrono::time_point<std::chrono::system_clock> m_recStart;
   std::chrono::time_point<std::chrono::system_clock> m_pauseStart;
   std::string m_scriptFileName;

   std::chrono::time_point<std::chrono::system_clock> m_featureStart;
   std::chrono::time_point<std::chrono::system_clock> m_featurePauseStart;
   
   static void* scriptThread(void* cntrlPointer);
   static void* theatreThread(void* cntrlPointer);


public:
   Script();
   
   void initializeScript(Controller* cntrl);
   void beginRecordScript(std::string scriptName, std::string featDur, std::string endCred);
   void recordCommand(const CommandStruct& cs);
   void stopRecordScript(void);
   void startScript(std::string fileName);
   void pauseScript(void);
   void continueScript(void);
   void stopScript(void);
   void startTheatreProgram(std::string scriptName, std::chrono::duration<double> secUntilTheatreStart);
   void abortTheatreProgram(void);
   void pauseTheatreProgram(void);
   void continueTheatreProgram(void);
   
};






#endif