#ifndef SCRIPT_H
#define SCRIPT_H

#include <chrono>


class Controller;

enum t_scriptStates
{
   idle,
   recording,
   playing,
   paused
};

class ScheduledCommand
{
public:
   std::chrono::duration<double> m_time;
   std::string m_command;
   
   ScheduledCommand(std::chrono::duration<double> time, std::string command) :
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
   std::string m_scriptFileName;
   
   
   static void* scriptThread(void* cntrlPointer);


public:
   Script();
   
   void initializeScript(Controller* cntrl);
   void beginRecordScript(std::string scriptName);
   void recordCommand(std::string command);
   void stopRecordScript(void);
   void startScript(std::string fileName);
   void pauseScript(void);
   void continueScript(void);
   void stopScript(void);
   
};






#endif