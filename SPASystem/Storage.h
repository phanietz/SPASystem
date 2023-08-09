#ifndef STORAGE_H
#define STORAGE_H
#include <Arduino.h>

class Storage{
  private:
    unsigned int address=0;
    byte data=0xFF;
    int INTEGER=0, DECIMAL=0;
    float axis=0;
  public:
    Storage();
    void WriteFLOAT(int motor, float axis);
    void Write(unsigned int dataAddres, byte dataVal);
    float ReadFLOAT(int motor);
    byte Read(unsigned int dataAddres);
    //bool DetectSignal();
};


#endif
