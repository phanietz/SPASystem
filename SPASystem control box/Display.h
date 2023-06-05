#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>

class Display{
  private:
    int status;
    const int LCD_COLS = 20;
    const int LCD_ROWS = 4;
  public:
      Display(); 
      void SPA_System();
      void screenOn(String title, int motor, float axis, int stepNum);      
      void MainScreen1(float axis1, float axis2, float axis3);
      void MainScreen2(String title, int motor, float axis, int stepNum);
      void setStep(float axis);
      void stepLength(int stepNum);
      void reset(int motor, float axis);
      void resetPosition(float axis);      
      void AskareYouSure();
      void successful(int message); //Reset and Reboot messages
      void AskgoBack();
      void donotSaveChangesMESSAGE();
      void returning(float axis);
      void AskReboot();
      void Reboot_HomePosition();
      void Reboot_LastPosition(float last, float axis);
      void rebootPosition(float axis);
      void LostSignal();
      bool AskActivateRemoteControl(class USBHost &usb2, unsigned long &keyPress);
      bool AskDeactivateRemoteControl(class USBHost &usb2, unsigned long &keyPress);
      void RemoteControl(int state);
};

#endif
