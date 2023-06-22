#ifndef REMOTEACCESSCONTROL_H
#define REMOTEACCESSCONTROL_H
#include <Arduino.h>

class RAC{
  private:
    char a;
    String responseAT="";
  public:
      RAC(); 
      //void pruebaRemote(class Display &dis);
      String RESET();
      String INIT();
      String INQ(bool mode);
      void statusPAIR();
};

#endif
