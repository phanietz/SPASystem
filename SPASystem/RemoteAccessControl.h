#ifndef REMOTEACCESSCONTROL_H
#define REMOTEACCESSCONTROL_H
#include <Arduino.h>

class RAC{
  private:
    char a;
    String responseAT="";
  public:
      RAC(); //init bluetooth
      //void pruebaRemote(class Display &dis);
      void INITIALIZED();
      String RESET(bool mode);
      String INIT(bool mode);
      String INQ(bool mode);
      void SendCopy();
      
};

#endif
