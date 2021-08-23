#ifndef IRCC_H
#define IRCC_H

#include <vector>

//  <command name="Confirm" type="ircc" value="AAAAAwAAHFoAAAA9Aw==" />
//  <command name="Up" type="ircc" value="AAAAAwAAHFoAAAA5Aw==" />
//  <command name="Down" type="ircc" value="AAAAAwAAHFoAAAA6Aw==" />
//  <command name="Right" type="ircc" value="AAAAAwAAHFoAAAA8Aw==" />
//  <command name="Left" type="ircc" value="AAAAAwAAHFoAAAA7Aw==" />
//  <command name="Home" type="ircc" value="AAAAAwAAHFoAAABCAw==" />
//  <command name="Options" type="ircc" value="AAAAAwAAHFoAAAA/Aw==" />
//  <command name="Return" type="ircc" value="AAAAAwAAHFoAAABDAw==" />
//  <command name="Num1" type="ircc" value="AAAAAwAAHFoAAAAAAw==" />
//  <command name="Num2" type="ircc" value="AAAAAwAAHFoAAAABAw==" />
//  <command name="Num3" type="ircc" value="AAAAAwAAHFoAAAACAw==" />
//  <command name="Num4" type="ircc" value="AAAAAwAAHFoAAAADAw==" />
//  <command name="Num5" type="ircc" value="AAAAAwAAHFoAAAAEAw==" />
//  <command name="Num6" type="ircc" value="AAAAAwAAHFoAAAAFAw==" />
//  <command name="Num7" type="ircc" value="AAAAAwAAHFoAAAAGAw==" />
//  <command name="Num8" type="ircc" value="AAAAAwAAHFoAAAAHAw==" />
//  <command name="Num9" type="ircc" value="AAAAAwAAHFoAAAAIAw==" />
//  <command name="Num0" type="ircc" value="AAAAAwAAHFoAAAAJAw==" />
//  <command name="Power" type="ircc" value="AAAAAwAAHFoAAAAVAw==" />
//  <command name="Display" type="ircc" value="AAAAAwAAHFoAAABBAw==" />
//  <command name="Audio" type="ircc" value="AAAAAwAAHFoAAABkAw==" />
//  <command name="SubTitle" type="ircc" value="AAAAAwAAHFoAAABjAw==" />
//  <command name="Favorites" type="ircc" value="AAAAAwAAHFoAAABeAw==" />
//  <command name="Yellow" type="ircc" value="AAAAAwAAHFoAAABpAw==" />
//  <command name="Blue" type="ircc" value="AAAAAwAAHFoAAABmAw==" />
//  <command name="Red" type="ircc" value="AAAAAwAAHFoAAABnAw==" />
//  <command name="Green" type="ircc" value="AAAAAwAAHFoAAABoAw==" />
//  <command name="Play" type="ircc" value="AAAAAwAAHFoAAAAaAw==" />
//  <command name="Stop" type="ircc" value="AAAAAwAAHFoAAAAYAw==" />
//  <command name="Pause" type="ircc" value="AAAAAwAAHFoAAAAZAw==" />
//  <command name="Rewind" type="ircc" value="AAAAAwAAHFoAAAAbAw==" />
//  <command name="Forward" type="ircc" value="AAAAAwAAHFoAAAAcAw==" />
//  <command name="Prev" type="ircc" value="AAAAAwAAHFoAAABXAw==" />
//  <command name="Next" type="ircc" value="AAAAAwAAHFoAAABWAw==" />
//  <command name="Replay" type="ircc" value="AAAAAwAAHFoAAAB2Aw==" />
//  <command name="Advance" type="ircc" value="AAAAAwAAHFoAAAB1Aw==" />
//  <command name="Angle" type="ircc" value="AAAAAwAAHFoAAABlAw==" />
//  <command name="TopMenu" type="ircc" value="AAAAAwAAHFoAAAAsAw==" />
//  <command name="PopUpMenu" type="ircc" value="AAAAAwAAHFoAAAApAw==" />
//  <command name="Eject" type="ircc" value="AAAAAwAAHFoAAAAWAw==" />
//  <command name="Karaoke" type="ircc" value="AAAAAwAAHFoAAABKAw==" />
//  <command name="Netflix" type="ircc" value="AAAAAwAAHFoAAABLAw==" />
//  <command name="Mode3D" type="ircc" value="AAAAAwAAHFoAAABNAw==" />


enum irccCode_t
{
  irccConfirm,
  irccUp,
  irccDown,
  irccRight,
  irccLeft,
  irccHome,
  irccOptions,
  irccReturn,
  irccNum1,
  irccNum2,
  irccNum3,
  irccNum4,
  irccNum5,
  irccNum6,
  irccNum7,
  irccNum8,
  irccNum9,
  irccNum0,
  irccPower,
  irccDisplay,
  irccAudio,
  irccSubTitle,
  irccFavorites,
  irccYellow,
  irccBlue,
  irccRed,
  irccGreen,
  irccPlay,
  irccStop,
  irccPause,
  irccRewind,
  irccForward,
  irccPrev,
  irccNext,
  irccReplay,
  irccAdvance,
  irccAngle,
  irccTopMenu,
  irccPopUpMenu,
  irccEject,
  irccKaraoke,
  irccNetflix,
  irccMode3D,
  irccLastIrccCode
};




class Ircc
{
private:

   std::vector<std::string> m_irccCmnds;


   void getDial(void);
   void getIrcc(void);
   void getDmr(void);
   void registerNoAuth(void);
   void registerAuth(std::string authStr);
   void sendPause(void);
   void sendPlay(void);
   
   
public:   
   Ircc();

   void initialize(void);
   void test(void);

   void wakeOnLan(void);
   void sendCode(irccCode_t code);
};

#endif