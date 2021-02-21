
#include "comms.h"

enum YamahaSourcesEnum
{
   playStation,
   chromecast,
   television,
   vinyl,
   spotify,
   tuner,
   aux,
   usb,
   bluRay,
   raspberryPi,
   lastSource
};

class Ynca
{
private:
   Comms* m_comms;
   char m_reply[2000];
   static const char* const sourceStrings[];
   
   void turnOn(void);
   
public:   
   Ynca(Comms* comms);

   void initialize(void);
   bool isYamahaOn(void);
   void turnOff(void);
   void startSource(YamahaSourcesEnum source);
   std::string getVolume(void);
   void setVolume(std::string vol);
   void volUp(void);
   void volDown(void);
   YamahaSourcesEnum getCurrentSource(void);
};
