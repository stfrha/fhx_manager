
class Benq
{
private:
   int m_fd;
   char m_reply[200];


   int communicateBenqRequest(const char* cmd, char* reply, int maxReplyLen);

public:   
   Benq();
   
   bool initializeBenq(void);
   void doDummyCommunication(void);
   bool isBenqOn(void);
   void turnOn(void);
   void turnOff(void);
   
   

//#define COMMISSIONING_TEST





};
