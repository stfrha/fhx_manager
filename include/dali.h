#ifndef DALI_H
#define DALI_H

class Dali
{
private:
   int m_fd;
   unsigned int m_randomAddress1 = 0xdeadbe;
   unsigned int m_randomAddress2 = 0x00dead;
   unsigned int m_rxSearchAddress = 0;
   bool m_withdrawn1 = false;
   bool m_withdrawn2 = false;

   
   void sendSearchAddressHigh(unsigned int searchAddress);
   void sendSearchAddressMid(unsigned int searchAddress);
   void sendSearchAddressLow(unsigned int searchAddress);
   bool compareSearchAddress(void);
   void queryShortAddress(void);
   void withdraw(void);
   void queryChannel(int channel);
   void communicateDaliHatCommand(const char* cmd, int respN, char* resp);
   int communicateDaliCommand(const char* cmd);


public:   
   Dali();
   
   bool initializeDali(void);
   void terminate(void);
   // newAddr = -1: original first-time commissioning (all drivers, addresses 0 and 1)
   // newAddr = 0..63: commission only drivers WITHOUT a short address, assign newAddr
   void commisioningProtocol(int newAddr = -1);
   void setLightPower(unsigned int channel, unsigned int power);
   void setFadeTime(unsigned int channel, unsigned int fadeTime);
   void broadcastLightPower(unsigned int power);
   bool isLightsOn(void);
   
   

//#define COMMISSIONING_TEST

};

#endif

