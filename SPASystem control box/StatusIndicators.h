#ifndef STATUSINDICATORS_H
#define STATUSINDICATORS_H
#include <Arduino.h>

class Colors{
  public:
    int control=0; //0=keyboard   1=remote
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
    void Control(); //keyboard or remote
    void White();
    void ChangeMoving();
    void info();
};

#endif
