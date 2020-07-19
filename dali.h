
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
   void commisioningProtocol(void);
   void queryChannel(int channel);
   void communicateCommand(const char* cmd, int respN, char* resp);
   int commissioningCommunication(const char* cmd);


public:   
   Dali();
   
   bool initializeDali(void);
   void terminate(void);
   void commisioning(void);
   void setLightPower(unsigned char channel, unsigned char power);
   
   

//#define COMMISSIONING_TEST





};
