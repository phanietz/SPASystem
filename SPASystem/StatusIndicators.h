#ifndef STATUSINDICATORS_H
#define STATUSINDICATORS_H
#include <Arduino.h>

class Colors{
  public:
    int control=1; //1=Keyboard   2=Infrared   =Bluetooth
  public:
    Colors(); 
    void testColor();
    void WeatherTech();
    void Error();
    void Warning(); //when the motor go back
    void Question();
    void Successful(); //saved successful
    void Move();
    void PushButtom();
    void Control(); //keyboard, Infrared remote or Bluetooth remote
    void White();
    void ChangeMoving();
    void info();
};

#endif
