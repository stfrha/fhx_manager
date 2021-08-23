#ifndef COMMANDS_H
#define COMMANDS_H

#include <vector>
#include <string>

enum t_commands
{
   statusReq = 0x00,
   lightAllOn = 0x01,
   lightAllOff = 0x02,
   lightPreMovie = 0x03,
   lightMovie = 0x04,
   lightPause = 0x05,
   lightKidsMovie = 0x06,
   lightEndCred = 0x07,
   ledStrip = 0x08,
   system_off = 0x10,
   volUp = 0x11,
   volDown = 0x12,
   volSet = 0x13,
   sourcePs = 0x14,
   sourceCc = 0x15,
   sourceTv = 0x16,
   sourceSpotify = 0x17,
   sourceVinyl = 0x18,
   sourceTuner = 0x19,
   sourceRpi = 0x1a,
   sourceAux = 0x1b,
   sourceBr = 0x1c,
   dilogPower = 0x20,
   dilogMute = 0x21,
   dilogList = 0x22,
   dilogTime = 0x23,
   dilog1 = 0x24,
   dilog2 = 0x25,
   dilog3 = 0x26,
   dilog4 = 0x27,
   dilog5 = 0x28,
   dilog6 = 0x29,
   dilog7 = 0x2a,
   dilog8 = 0x2b,
   dilog9 = 0x2c,
   dilog0 = 0x2d,
   dilogInfo = 0x2e,
   dilogMenu = 0x2f,
   dilogEpg = 0x30,
   dilogUp = 0x31,
   dilogExit = 0x32,
   dilogLeft = 0x33,
   dilogOk = 0x34,
   dilogRight = 0x35,
   dilogSubtitle = 0x36,
   dilogDown = 0x37,
   dilogText = 0x38,
   dilogStop = 0x39,
   dilogPlay = 0x3a,
   dilogPause = 0x3b,
   dilogRecord = 0x3c,
   dilogRed = 0x3d,
   dilogGreen = 0x3e,
   dilogYellow = 0x3d,
   dilogBlue = 0x40,
   dilogNext = 0x41,
   dilogPrevious = 0x42,
   bdPower = 0x50,
   bdSubtitle = 0x51,
   bdEject = 0x52,
   bdRewind = 0x53,
   bdPlay = 0x54,
   bdFf = 0x55,
   bdPrevious = 0x56,
   bdPause = 0x57,
   bdStop = 0x58,
   bdNext = 0x59,
   bdTopMenu = 0x5a,
   bdUp = 0x5b,
   bdBack = 0x5c,
   bdLeft = 0x5d,
   bdOk = 0x5e,
   bdRight = 0x5f,
   bdPopupMenu = 0x60,
   bdDown = 0x70,
   bdOptions = 0x71,
   bdRed = 0x72,
   bdGreen = 0x73,
   bdYellow = 0x74,
   bdBlue = 0x75,
   bdPowerOnLan = 0x76,
   playScript = 0x80,
   pauseScript = 0x81,
   stopPlayScript = 0x82,
   recScript = 0x83,
   stopRecScript = 0x84,
   continueScript = 0x85,
   startTheatreProgram = 0x86,
   abortTheatreProgram = 0x87,
   pauseTheatreProgram = 0x88,
   continueTheatreProgram = 0x89,
};

class CommandStruct
{
public:
   static const int c_magic = 0x5a;
   t_commands m_id;
   std::string m_argument;
   
   CommandStruct();
   CommandStruct(t_commands id, std::string argument);
   
};


#endif