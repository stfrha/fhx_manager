#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <vector>
#include <unistd.h>
#include <sstream>
#include <experimental/filesystem>

// For mutex and inter-thread communication
#include <mutex>
#include <condition_variable>

// For threads
#include <pthread.h>

#include "script.h"

#include "controller.h"

using namespace std;

// -----------------------------------------------------------------------
// Shared variables for thread communication

// Script thread
mutex scriptMu;
condition_variable scriptCondVar;
bool scriptGo;
bool scriptFinished;
t_scriptStates scriptState;
string scriptFileName;
string featureDuration;
string endCredits;
chrono::time_point<std::chrono::system_clock> playStart;

// Theatre thread
mutex theatreMu;
condition_variable theatreCondVar;
bool theatreGo;
bool theatreFinished;
t_theatreStates theatreState;
string theatreFileName;
chrono::time_point<std::chrono::system_clock> featureStart;
chrono::duration<double> theatreStartTimeout;
chrono::duration<double> crFeatureDuration;
chrono::duration<double> crEndCredits;

// -----------------------------------------------------------------------



Script::Script() :
   m_controller(NULL)
{
}

void Script::initializeScript(Controller* cntrl)
{
   m_controller = cntrl;

   pthread_t scriptThreadId;
   scriptState = t_scriptStates::scriptIdle;
   scriptFileName = "##noscript##";

   pthread_t theatreThreadId;
   theatreState = t_theatreStates::theatreIdle;
   theatreFileName = "##notheatre##";

   int result = pthread_create(&scriptThreadId, NULL, scriptThread, (void*)cntrl);
   if (result)
   {
      cout << "Script thread could not be created, " << result << endl;
      exit(1);
   }

   result = pthread_create(&theatreThreadId, NULL, theatreThread, (void*)cntrl);
   if (result)
   {
      cout << "Theatre thread could not be created, " << result << endl;
      exit(1);
   }
}

// https://en.cppreference.com/w/cpp/thread/condition_variable

void* Script::scriptThread(void* cntrlPointer)
{
   vector<ScheduledCommand> scheduledCommands;
   int scriptIndex = 0;

   Controller* cntrl = (Controller*)cntrlPointer;

   cout << "Script thread started." << endl;

   while (true)
   {
      if (scriptState == t_scriptStates::playing)
      {
         
         if (scriptIndex >= scheduledCommands.size())
         {
            cout << "End of script reached." << endl;

            scriptState = t_scriptStates::scriptIdle;
        }
         else
         {
            chrono::duration<double> to = scheduledCommands[scriptIndex].m_time - (chrono::system_clock::now() - playStart);

            cout << "Will wait for: " << to.count() << " for next command." << endl;

            unique_lock<mutex> lk(scriptMu);

            if (scriptCondVar.wait_for(lk, to, []{return scriptGo;}))
            {
               cout << "Thread got a kick while waiting for next command timeout" << endl;
               scriptGo = false;
               scriptFinished = true;
               lk.unlock();
               scriptCondVar.notify_one();
            }
            else
            {
               cout << "Script thread timed out, sending command: " << scheduledCommands[scriptIndex].m_command.m_id << endl;
               cntrl->executeCommand(scheduledCommands[scriptIndex].m_command);
               scriptIndex++;
            }
         }
      }
      else
      {
         cout << "Thread waiting for new start" << endl;
         unique_lock<mutex> lk(scriptMu);

         scriptCondVar.wait(lk, []{return scriptGo;});

         cout << "Thread got a kick while waiting for start" << endl;
         scriptGo = false;
         scriptFinished = true;
         
         lk.unlock();
         scriptCondVar.notify_one();
         
         if (scriptState == t_scriptStates::playing)
         {
            // We now starting a script
            cout << "Now starting to play script" << endl;
            
            // Read script
            scheduledCommands.clear();
            
            ifstream file("/home/pi/fhx_manager/scripts/" + scriptFileName);
            string line;
            
            if (getline(file, line))
            {
               int cmdDel = line.find(':');
               string cmd = line.substr(0, cmdDel);
               if (cmd != "fd")
               {
                  cout << "Error parsing script, feature duration not found" << endl;
               }
               
               string arg = line.substr(cmdDel + 1);
               
               int argDel1 = arg.find_first_of('.');
               int argDel2 = arg.find_last_of('.');
               
               string h = arg.substr(0, argDel1);
               string m = arg.substr(argDel1+1, argDel2 - argDel1 - 1);
               string s = arg.substr(argDel2+1);
                  
               crFeatureDuration = chrono::duration<double>(stod(h) * 3600.0 + stod(m) * 60 + stod(s));
            }

            if (getline(file, line))
            {
               int cmdDel = line.find(':');
               string cmd = line.substr(0, cmdDel);
               if (cmd != "ec")
               {
                  cout << "Error parsing script, end credits not found" << endl;
               }
               
               string arg = line.substr(cmdDel + 1);
               
               int argDel1 = arg.find_first_of('.');
               int argDel2 = arg.find_last_of('.');
               
               string h = arg.substr(0, argDel1);
               string m = arg.substr(argDel1+1, argDel2 - argDel1 - 1);
               string s = arg.substr(argDel2+1);
                  
               crEndCredits = chrono::duration<double>(stod(h) * 3600.0 + stod(m) * 60 + stod(s));
            }
             
            while (getline(file, line))
            {
               int cmdDel = line.find(';');
               string cmd = line.substr(0, cmdDel);
               t_commands cmdId = (t_commands)stoi(cmd);
               int durDel = line.find_last_of('@');
               string arg = line.substr(cmdDel + 1, durDel - cmdDel - 1);
               string t = line.substr(durDel+1);
               
               
               ScheduledCommand sc(chrono::duration<double>(stod(t)), CommandStruct(cmdId, arg));
               scheduledCommands.push_back(sc);
            }
            
            scriptIndex = 0;
            
         }
      }
   }
}


/* Some notes

To change source: 14 sec
To cold start system: 42 sec

*/



void* Script::theatreThread(void* cntrlPointer)
{
   Controller* cntrl = (Controller*)cntrlPointer;

   cout << "Theatre thread started." << endl;

   while (true)
   {
      if (theatreState == t_theatreStates::theatreIdle)
      {
         cout << "Theatre thread waiting for new start" << endl;
         unique_lock<mutex> lk(theatreMu);

         theatreCondVar.wait(lk, []{return theatreGo;});

         cout << "theatre thread got a kick while waiting for start" << endl;
         theatreGo = false;
         theatreFinished = true;
         
         lk.unlock();
         theatreCondVar.notify_one();
         
         if (theatreState == t_theatreStates::theatreArmed)
         {
            // We now start execution of the theatre show, including initial wait
            
         }
      }
      else if (theatreState == t_theatreStates::theatreArmed)
      {
         // ---------------------------------------------------------------------
         // Step 1:
         // Set light to all on
         // Start blue-ray cuing
         // Start RPI source
         // Start playing crowd movie.
         // ---------------------------------------------------------------------
         {
            cout << "Will wait for: " << theatreStartTimeout.count() << " for next step." << endl;

            unique_lock<mutex> lk(theatreMu);

            if (theatreCondVar.wait_for(lk, theatreStartTimeout, []{return theatreGo;}))
            {
               cout << "Thread got a kick while waiting for next command timeout" << endl;
               theatreGo = false;
               theatreFinished = true;
               lk.unlock();
               theatreCondVar.notify_one();
               continue;
            }
            else
            {
               cout << "Theatre thread timed out, set light to all on, start blue-ray cuing, start RPI source. Start playing crowd movie." << endl;
               cntrl->executeCommand(CommandStruct(t_commands::playScript, theatreFileName));
               cntrl->executeCommand(CommandStruct(t_commands::lightAllOn, ""));
               cntrl->setBlackScreen();
               cntrl->executeCommand(CommandStruct(t_commands::sourceRpi, ""));
               string cmd = "omxplayer -o hdmi \"/home/pi/fhx_manager/video/crowd.mp4\"";
               system( cmd.c_str() );

               cout << "Crowd movie finished, lower lights to pre-movie and start commercials" << endl;
               cntrl->executeCommand(CommandStruct(t_commands::lightPreMovie, ""));
               
               //  Play 3 Commercials
               vector<string> files;
               vector<int> list; 
               int i = 0;
               string path = "/home/pi/fhx_manager/video/commercials";
               for (const auto & entry : experimental::filesystem::directory_iterator(path))
               {
                  files.push_back(entry.path());
                  list.push_back(i++);
                  cout << "Found commercial file: " << entry.path() << endl;
               }
               cout << "Found " << files.size() << " commercial videos" << endl;

               // Now randomize order
               for (int i = 0; i < list.size(); i++)
               {
                  // Swap position i with a random number
                  int r = rand() % list.size();
                  int t = list[i];
                  list[i] = list[r];
                  list[r] = t;
               }
               
               // Now play the three first files
               for (int i = 0; i < min(3, (int)list.size()); i++)
               {
                  cout << files[list[i]] << endl;
                  string cmd = "omxplayer -o hdmi \"" + files[list[i]] + "\"";
                  system( cmd.c_str() );
                  cmd = "omxplayer -o hdmi \"/home/pi/fhx_manager/video/black_screen.mp4\"";
                  system( cmd.c_str() );
               }

               cntrl->executeCommand(CommandStruct(t_commands::sourceBr, ""));
            }
         }

         // ---------------------------------------------------------------------
         // Step 2, 
         // Wait source change
         // Start feature 
         // Lights off
         // ---------------------------------------------------------------------
         {
            unique_lock<mutex> lk(theatreMu);

            if (theatreCondVar.wait_for(lk, 14*1000ms, []{return theatreGo;}))
            {
               cout << "Thread got a kick while waiting for next command timeout" << endl;
               theatreGo = false;
               theatreFinished = true;
               lk.unlock();
               theatreCondVar.notify_one();
               continue;
            }
            else
            {
               cout << "Theatre thread timed out, Start feature." << endl;
               cntrl->executeCommand(CommandStruct(t_commands::bdPlay, ""));
               theatreState = t_theatreStates::featureRunning;
               featureStart = chrono::system_clock::now();
            }
         }

      }
      else if (theatreState == t_theatreStates::featureRunning)
      {
         unique_lock<mutex> lk(theatreMu);

         chrono::duration<double> to = crEndCredits - (chrono::system_clock::now() - featureStart);

         if (theatreCondVar.wait_for(lk, to, []{return theatreGo;}))
         {
            cout << "Thread got a kick while waiting for next command timeout" << endl;
            theatreGo = false;
            theatreFinished = true;
            lk.unlock();
            theatreCondVar.notify_one();

            // Check if this is a pause, in this case, we shall pause movie
            // and set lights to all on.
            if (theatreState == t_theatreStates::featurePaused)
            {
               cout << "Feature movie paused." << endl;
               cntrl->executeCommand(CommandStruct(t_commands::bdPause, ""));
               cntrl->executeCommand(CommandStruct(t_commands::lightAllOn, ""));
            }
            
            continue;
         }
         else
         {
            cout << "Theatre thread timed out, Lights to end credits." << endl;
            cntrl->executeCommand(CommandStruct(t_commands::lightEndCred, ""));
            theatreState = t_theatreStates::theatreEnd;
         }
      }
      else if (theatreState == t_theatreStates::theatreEnd)
      {
         // ---------------------------------------------------------------------
         // Step 5, 
         // ---------------------------------------------------------------------
         
         unique_lock<mutex> lk(theatreMu);

         chrono::duration<double> to = crFeatureDuration - (chrono::system_clock::now() - featureStart);

         if (theatreCondVar.wait_for(lk, to, []{return theatreGo;}))
         {
            cout << "Thread got a kick while waiting for next command timeout" << endl;
            theatreGo = false;
            theatreFinished = true;
            lk.unlock();
            theatreCondVar.notify_one();

            // Check if this is a pause, in this case, we shall pause movie
            // and set lights to all on.
            if (theatreState == t_theatreStates::featurePaused)
            {
               cout << "Feature movie paused." << endl;
               cntrl->executeCommand(CommandStruct(t_commands::bdPause, ""));
               cntrl->executeCommand(CommandStruct(t_commands::lightAllOn, ""));
            }
            
            continue;
         }
         else
         {
            cout << "Theatre thread timed out, Lights to all on." << endl;
            cntrl->executeCommand(CommandStruct(t_commands::lightAllOn, ""));
            theatreState = t_theatreStates::theatreIdle;
         }
      }
      else if (theatreState == t_theatreStates::featurePaused)
      {
         unique_lock<mutex> lk(theatreMu);

         theatreCondVar.wait(lk, []{return theatreGo;});

         cout << "Thread got a kick while waiting for continue from pause" << endl;
         theatreGo = false;
         theatreFinished = true;
         lk.unlock();
         theatreCondVar.notify_one();

         // Check if this is a featureRunning, in this case, we shall start movie 
         // and set lights to all off.
         if (theatreState == t_theatreStates::featureRunning)
         {
            cout << "Feature movie paused." << endl;
            cntrl->executeCommand(CommandStruct(t_commands::bdPlay, ""));
            cntrl->executeCommand(CommandStruct(t_commands::lightAllOff, ""));
         }
      }
   }
}


void Script::beginRecordScript(std::string scriptName, std::string featDur, std::string endCred)
{

   // TODO: Handle safeguard if script is playing
  
   m_recStart = chrono::system_clock::now();
   scriptState = t_scriptStates::recording;
   scriptFileName = scriptName;
   featureDuration = featDur;
   endCredits = endCred;
   
   cout << "Starting to record commands to: " << scriptName << endl;
   ofstream outfile;
   string file = "/home/pi/fhx_manager/scripts/" + scriptFileName;
   outfile.open(file.c_str());
   outfile << "fd:" << featureDuration << endl;
   outfile << "ec:" << endCredits << endl;
   outfile.close();
}

void Script::recordCommand(const CommandStruct& cs)
{
   if (scriptState == t_scriptStates::recording)
   {
      auto tNow = chrono::system_clock::now();
      
      chrono::duration<double> dur = tNow - m_recStart;
      
      // Test so we do not record any canibalistic commands (script related). 
      if (((cs.m_id < t_commands::playScript) || (cs.m_id < t_commands::stopRecScript)) && (cs.m_id != t_commands::statusReq))
      {
         cout << "Record command: " << cs.m_id << ";" << cs.m_argument << "@" << dur.count() << endl;
         
         ofstream outfile;
         string file = "/home/pi/fhx_manager/scripts/" + scriptFileName;
         outfile.open(file.c_str(), std::ios_base::app);
         outfile << cs.m_id << ";" << cs.m_argument << "@" << dur.count() << endl;
         outfile.close();
      }
   }
   else
   {
      cout << "Got command: " << cs.m_id << ", but recording is not on" << endl;
   }
}

void Script::stopRecordScript(void)
{
   scriptState = t_scriptStates::scriptIdle;
   
   cout << "Stopping recording of script" << endl;
}

void Script::startScript(std::string fileName)
{
   if (scriptState == t_scriptStates::scriptIdle)
   {
      playStart = chrono::system_clock::now();

      cout << "Giving script thread a kick!" << endl;

      {
         lock_guard<mutex> lk(scriptMu);
         scriptGo = true;
         scriptState = t_scriptStates::playing;
         scriptFileName = fileName;
      }

      cout << "About to notify thread" << endl;

      scriptCondVar.notify_one();
    
      cout << "Waiting for thread response..." << endl;

      {
         unique_lock<mutex> lk(scriptMu);
         scriptCondVar.wait(lk, []{return scriptFinished;});
         scriptFinished = false;
      }

      cout << "Got script thread response" << endl;
   }
}

void Script::pauseScript(void)
{
   // TODO: correct start time
   m_pauseStart = chrono::system_clock::now();

   cout << "Giving script thread a kick!" << endl;

   {
      lock_guard<mutex> lk(scriptMu);
      scriptGo = true;
      scriptState = t_scriptStates::paused;
   }

   cout << "About to notify thread" << endl;

   scriptCondVar.notify_one();
 
   cout << "Waiting for thread response..." << endl;

   {
      unique_lock<mutex> lk(scriptMu);
      scriptCondVar.wait(lk, []{return scriptFinished;});
      scriptFinished = false;
   }

   cout << "Got script thread response" << endl;
   
}

void Script::continueScript(void)
{
   if (scriptState == t_scriptStates::paused)
   {
      std::chrono::time_point<std::chrono::system_clock> now = chrono::system_clock::now();
      // Reset start time as if the pause never happened
      // i.e. add pause duration to start time
      playStart += now - m_pauseStart;
      

      cout << "Giving script thread a kick!" << endl;

      {
         lock_guard<mutex> lk(scriptMu);
         scriptGo = true;
         scriptState = t_scriptStates::playing;
      }

      cout << "About to notify thread" << endl;

      scriptCondVar.notify_one();
    
      cout << "Waiting for thread response..." << endl;

      {
         unique_lock<mutex> lk(scriptMu);
         scriptCondVar.wait(lk, []{return scriptFinished;});
         scriptFinished = false;
      }

      cout << "Got script thread response" << endl;
   }
}

void Script::stopScript(void)
{
   cout << "Giving script thread a kick!" << endl;

   {
      lock_guard<mutex> lk(scriptMu);
      scriptGo = true;
      scriptState = t_scriptStates::scriptIdle;
   }

   cout << "About to notify thread" << endl;

   scriptCondVar.notify_one();
 
   cout << "Waiting for thread response..." << endl;

   {
      unique_lock<mutex> lk(scriptMu);
      scriptCondVar.wait(lk, []{return scriptFinished;});
      scriptFinished = false;
   }

   cout << "Got script thread response" << endl;
   
}

void Script::startTheatreProgram(std::string scriptName, chrono::duration<double> secUntilTheatreStart)
{
   if (theatreState == t_theatreStates::theatreIdle)
   {
      cout << "About to start feature with script: " << scriptName << ", after duration: " << secUntilTheatreStart.count() << endl;
      cout << "Giving theatre thread a kick!" << endl;

      {
         lock_guard<mutex> lk(theatreMu);
         theatreGo = true;
         theatreState = t_theatreStates::theatreArmed;
         theatreStartTimeout = secUntilTheatreStart;
         theatreFileName = scriptName;
      }

      cout << "About to notify thread" << endl;

      theatreCondVar.notify_one();
    
      cout << "Waiting for thread response..." << endl;

      {
         unique_lock<mutex> lk(theatreMu);
         theatreCondVar.wait(lk, []{return theatreFinished;});
         theatreFinished = false;
      }

      cout << "Got theatre thread response" << endl;
   }
}

void Script::abortTheatreProgram(void)
{
   stopScript();
   
   cout << "Giving theatre thread a kick!" << endl;

   {
      lock_guard<mutex> lk(theatreMu);
      theatreGo = true;
      theatreState = t_theatreStates::theatreIdle;
   }

   cout << "About to notify thread" << endl;

   theatreCondVar.notify_one();
 
   cout << "Waiting for thread response..." << endl;

   {
      unique_lock<mutex> lk(theatreMu);
      theatreCondVar.wait(lk, []{return theatreFinished;});
      theatreFinished = false;
   }

   cout << "Got theatre thread response" << endl;
   
}

void Script::pauseTheatreProgram(void)
{
   if (theatreState != t_theatreStates::featureRunning)
   {
      return;      
   }

   m_featurePauseStart = chrono::system_clock::now();
   
   cout << "Giving theatre thread a kick!" << endl;

   {
      lock_guard<mutex> lk(theatreMu);
      theatreGo = true;
      theatreState = t_theatreStates::featurePaused;
   }

   cout << "About to notify thread" << endl;

   theatreCondVar.notify_one();
 
   cout << "Waiting for thread response..." << endl;

   {
      unique_lock<mutex> lk(theatreMu);
      theatreCondVar.wait(lk, []{return theatreFinished;});
      theatreFinished = false;
   }

   cout << "Got theatre thread response" << endl;
}


void Script::continueTheatreProgram(void)
{
   if (theatreState == t_theatreStates::featurePaused)
   {
      std::chrono::time_point<std::chrono::system_clock> now = chrono::system_clock::now();
      // Reset start time as if the pause never happened
      // i.e. add pause duration to start time
      featureStart += now - m_featurePauseStart;
      

      cout << "Giving theatre thread a kick!" << endl;

      {
         lock_guard<mutex> lk(theatreMu);
         theatreGo = true;
         theatreState = t_theatreStates::featureRunning;
      }

      cout << "About to notify thread" << endl;

      theatreCondVar.notify_one();
    
      cout << "Waiting for thread response..." << endl;

      {
         unique_lock<mutex> lk(theatreMu);
         theatreCondVar.wait(lk, []{return theatreFinished;});
         theatreFinished = false;
      }

      cout << "Got theatre thread response" << endl;
   }
}

