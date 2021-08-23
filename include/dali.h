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
   void commisioningProtocol(void);
   void setLightPower(unsigned int channel, unsigned int power);
   void setFadeTime(unsigned int channel, unsigned int fadeTime);
   void broadcastLightPower(unsigned int power);
   bool isLightsOn(void);
   
   

//#define COMMISSIONING_TEST

};

#endif

