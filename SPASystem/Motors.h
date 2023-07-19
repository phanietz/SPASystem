#ifndef MOTORS_H
#define MOTORS_H
#include <Arduino.h>
//Switches for calibration
//Motor 1: PIN 40 (it's touched when the sensor is moving down)
//Motor 2: PIN 43 (it's touched when the sensor is moving backward)
//Motor 3: PIN 44 (it's touched when the sensor is moving left)

//Switches for end of te rail
//Motor 1: PIN 41 (it's touched when the sensor is moving up)
//Motor 2: PIN 42 (it's touched when the sensor is moving forward)
//Motor 3: PIN 44 (it's touched when the sensor is moving right)
class Motors{
  private:
    const int num_steps_UP = 40;  //switch 41
    const int num_steps_FORWARD = 14; //switch 42
    const int num_steps_LEFT = 16; //switch 45    
    int longStep=50;
    float lenght=0.50, aux=0;
    //*********reset position variables
    int auxUp=0,auxDown=0, auxLeft=0, auxRight=0, auxForward=0, auxBackward=0;
    bool reset = false;    
    //**********IRemote
    int RECV_PIN = 7;
    unsigned long key_value = 0; // variable to store the key value
    int goBack; 
    String LabelsX="  LABELS SENSOR (X) ", LabelsY="  LABELS SENSOR (Y) ", Conveyor="  CONVEYOR SENSOR   ";
  public: 
    int stepNum=2;
    int auxMotor=0;
    
  public:
    Motors();
    void start();
    float up(float CountSteps, bool unblocked);       //to move up
    float down(float CountSteps, bool unblocked);     //to move down
    float forward(float CountSteps, bool unblocked);
    float backward(float CountSteps, bool unblocked);    
    float left(float CountSteps, bool unblocked);     //to move left
    float right(float CountSteps, bool unblocked);    //to move right
    int Short(int stepNum);
    int Long(int stepNum);
    void stepLength(int stepNum);
    float ResetPositionMotorRemoteControl(class Display &dis, int motor, float axis, class IRrecv &IRremote, class decode_results &IRresult);
    float ResetPositionMotorKeyboard(class Display &dis, int motor, float axis, class USBHost &usb2, class KeyboardController &keyboard2, unsigned long &keyPress, unsigned long &keyRelease);
};


#endif
