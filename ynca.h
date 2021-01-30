
#include "comms.h"

enum YamahaSourcesEnum
{
   playStation,
   chromecast,
   television,
   turnTable,
   spotify,
   tuner,
   aux,
   usb,
   lastSource
};

class Ynca
{
private:
   Comms* m_comms;
   char m_reply[200];
   static const char* const sourceStrings[];
   
   bool isYamahaOn(void);
   void turnOn(void);
   
public:   
   Ynca(Comms* comms);

   void initialize(void);
   void turnOff(void);
   void startSource(YamahaSourcesEnum source);
   void volUp(void);
   void volDown(void);
};
