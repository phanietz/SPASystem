#define DECODE_NEC
#include <Arduino.h>
#include "Display.h"
#include "Motors.h"
#include "IRremote2.h"
#include "Storage.h"
#include <KeyboardController.h>
#include "RemoteAccessControl.h" //library test
#include "StatusIndicators.h"

//Position 0: 40, 43 y 44
#define SWITCH_M1_1 40  //brown Switch 1
#define SWITCH_M1_2 41  //gray Switch 1
#define SWITCH_M2_3 42  //brown Switch 2
#define SWITCH_M2_4 43  //gray Switch 2
#define SWITCH_M3_5 44  //brown Switch 3
#define SWITCH_M3_6 45  //gray Switch 3
int RECV_PIN = 7;
int RedButton = 2;
#define ST 13 //Bluetooth - STATE
#define EN 12 //Bluetooth - ENABLE
#define CH1 16753245 //To move motor 1, sensor of conveyor
#define CH2 16736925 //To move motor 2, sensor of the labels X axis
#define CH3 16769565 //To move motor 3, sensor of the labels Y axis
#define Up_Backward_Right 16720605 //To move sensor
#define Down_Forward_Left 16712445 //To move sensor
#define ENTER 16761405 //Enter
#define SLOW 16769055 //Slow  //NEXT to go Right
#define FAST 16754775 //Fast
#define EXIT 16748655 //Exit
#define RESET 16750695 //To Reset position
#define REBOOT 16756815 //To reboot
#define IR_remote 16738455 //To activate IR remote control
#define Repeat_ 4294967295 


/*------------------------------------------------*/
/*-----( Global variables )---------------*/
unsigned long keyPress=0, keyRelease=0; // variable to store the key value
bool setReset = false, signalMemory=false;
float axis1 = 0, axis2 = 0, axis3 = 0, a1, a2, a3;
int motor = 0;
int aux=1;
int screen=0;
volatile bool IRcontrol=false;
String LabelsX=" LABEL SENSOR    |x|", LabelsY=" LABEL SENSOR    |y|", Conveyor=" CONVEYOR SENSOR |z|";
unsigned long key_value = 0;
float last, temp; 
int UserInterface=1;

/////////////
//variables bluetooth
String responseAT="";
char a;
int touch=0; 
/*---------------------------------------*/
/*-----( Ojects )--- ------------*/  
IRrecv irrecv(RECV_PIN);
decode_results results;
USBHost usb;
KeyboardController keyboard(usb);
Display d1;
Motors m1;
Storage s1;
RAC Bluetooth;
//Colors color1;

/*---------------------------------------*/
/*-----( Punteros )---------------*/
//int * control = & color1.control;

/*---------------------------------------*/
/*-----( Functions )---------------*/
void ButtonKeyboard(int motor);
void ButtonRemoteControl(int motor);
void ButtonDisplayTouch(int motor);
bool detectSignal(int s);
void Reboot_y_n(unsigned long answer, int motor);

// This function intercepts key press
void keyPressed() {
  
  keyRelease=0; //start push buttom
  keyPress = keyboard.getOemKey();

  if(screen==1){
    switch(keyPress){
      case 32://motor1
              //**x*
              //****                    
              keyPress = CH3; //Select motor 3
              
              break;
      case 31://motor2
              //*x**
              //****
              keyPress = CH2; //Select motor 2
              break;              
      case 30://motor3
              //x***
              //****
              keyPress = CH1; //Select motor 1
              break;
    }
  }else if(screen==2){

    switch(keyPress){
      case 37://left,down,forward
              //****
              //***x      
              keyPress = Down_Forward_Left;
              break;
      case 36://step+
              //****
              //**x*
              keyPress = FAST;
              break;
      case 35://step-
              //****
              //*x**      
              keyPress = SLOW;
              break;
      case 34://exit
              //****
              //x***      
              keyPress = EXIT;
              Serial.println("EXIT");
              break;
      case 33://right,up,backward
              //***x
              //****      
              keyPress = Up_Backward_Right;
              break;
      case 32://reset
              //**x*
              //****         
              keyPress = RESET;        
              break;
      case 31://reboot
              //*x**
              //****      
              keyPress = REBOOT;               
              break;              
      case 30://enter
              //x***
              //****      
              keyPress = ENTER;
              Serial.println("ENTER");
              break;
    }    
  }
  //Serial.print("--Touch PRESS Event-- KEY: ");
  //Serial.println(keyPress);
}

// This function intercepts key release
void keyReleased() {
 //keyPress = 0;
 keyRelease=1; //stop push buttom
 //Serial.print("--Touch RELEASED Event--: "); 
 //Serial.println(keyRelease);
}

void setup() {  
  Serial.begin(9600);
  //SerialUSB.begin(115200);
  irrecv.enableIRIn(); // Start the receiver
  //*********pines switches************
  pinMode(SWITCH_M1_1, INPUT_PULLUP);
  pinMode(SWITCH_M1_2, INPUT_PULLUP);
  pinMode(SWITCH_M2_3, INPUT_PULLUP);
  pinMode(SWITCH_M2_4, INPUT_PULLUP);
  pinMode(SWITCH_M3_5, INPUT_PULLUP);
  pinMode(SWITCH_M3_6, INPUT_PULLUP);
  //*********INTERRUPTS****************  
  //attachInterrupt(digitalPinToInterrupt(RedButton), RedButtonInterrupt, RISING);  
  //**********ON/OFF IR****************  
  pinMode(RedButton, INPUT_PULLUP);
  //*********RAControl*****************
  pinMode(ST, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP); //RX3
  //pinMode(EN, OUTPUT); //reset Buetooth, State = o
  //digitalWrite(EN, HIGH); //reset Buetooth, State = o   
  Serial3.begin(38400); // Default communication rate of the Bluetooth module  

  //Bluetooth.pruebaRemote(d1); //pruebaaaaa
  signalMemory=detectSignal(0);
  UserInterface=1;
  //delay(1000);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//////////////////        LOOP       ////////////////////////////////
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void loop() {
  /*
  Keyboard:           UserInterface=1
  Infrared Remote:    UserInterface=2
  RAControl:          UserInterface=3
  */

  if(signalMemory==true){

    switch(UserInterface){
      case 1:
        ///////////////////////////////////////
        //          Keyboard loop            //
        ///////////////////////////////////////
        while(UserInterface==1){
          //**----About: Infrared Remote Control-----**//
          screen=1;
          usb.Task();  
          if (keyPress!=0){
            switch (keyPress) {
              case CH1:  //motor 1
                Serial.println("------------------MOTOR 1:");
                d1.screenOn(Conveyor, 1, axis1, m1.stepNum);        
                ButtonKeyboard(1);        
                break;
        
              case CH2:  //motor 2
                Serial.println("------------------MOTOR 2:");
                d1.screenOn(LabelsX, 2, axis2, m1.stepNum);
                ButtonKeyboard(2);        
                break;
        
              case CH3:  //motor 3
                Serial.println("------------------MOTOR 3:");
                d1.screenOn(LabelsY, 3, axis3, m1.stepNum);
                ButtonKeyboard(3);         
                break; 
            }
            keyPress=0;      
          }else if(digitalRead(RedButton)==false){ //RedButtonValue is ANALOG INPUT
            Serial.println("Asking, Do you want to ACTIVATE the remote control?");
            screen=2;
            IRcontrol=d1.AskActivateRemoteControl(usb, keyPress);
            if(IRcontrol==true){
              UserInterface=2;
            }
            screen=1;
            d1.MainScreen1(axis1, axis2, axis3);   
          }else if(digitalRead(ST)==true){       
            UserInterface=3;
            Serial.println("CONNECTED Bluetooth DEVICE");
            d1.RemoteControl(3);
            delay(500);            
            screen=1;
            d1.MainScreen1(axis1, axis2, axis3);            
          }
        }      
        break;

      case 2:
        ///////////////////////////////////////
        //  Infrared Remote Control loop     //
        ///////////////////////////////////////
        //here can't get acces from RAControl, just in keyboard loop        
        while(UserInterface==2){
          if (irrecv.decode(&results)){
            if (results.value == Repeat_) { // if the value is equal to 0xFFFFFFFF
              results.value = key_value; // set the value to the key value
            }      
            //Serial.println(String("LOOP value: ") + results.value);     
            switch (results.value) {
              case CH1:  //motor 1
                Serial.println("------------------MOTOR 1:-----------------");
                d1.screenOn(Conveyor, 1, axis1, m1.stepNum);        
                ButtonRemoteControl(1);
                break;
                
              case CH2:  //motor 2
                Serial.println("------------------MOTOR 2:-----------------");
                d1.screenOn(LabelsX, 2, axis2, m1.stepNum);
                ButtonRemoteControl(2);        
                break;
        
              case CH3:  //motor 3
                Serial.println("------------------MOTOR 3:-----------------");
                d1.screenOn(LabelsY, 3, axis3, m1.stepNum);
                ButtonRemoteControl(3);         
                break;
            }
            key_value = results.value;
            irrecv.resume(); // reset the receiver for the next code 
          }else if(digitalRead(RedButton)==false){ //RedButtonValue is ANALOG INPUT
            Serial.println("Asking, Do you want to DEACTIVATE the remote control?");
            screen=2;
            IRcontrol=d1.AskDeactivateRemoteControl(usb, keyPress);
            if(IRcontrol==false){
              UserInterface=1;
            }            
            screen=1;
            d1.MainScreen1(axis1, axis2, axis3);
          }
        }        
        break;

      case 3:
        ///////////////////////////////////////
        //          RAControl loop           //
        ///////////////////////////////////////
        Serial.print("imprime touch: ");
        Serial.println(touch);
        responseAT="";
        while(UserInterface==3){
          while(Serial3.available()>0){
            a = char(Serial3.read());
            responseAT += a;
          }    
          if(responseAT.indexOf(".")!=-1){
            responseAT=responseAT.substring(0, responseAT.length()-1); //remove \r\n from command string  
            Serial.println(responseAT);
            touch=responseAT.toInt();
            responseAT="";
          }
          switch (touch) {
            case CH1:  //motor 1
              Serial.println("------------------MOTOR 1:");
              d1.screenOn(Conveyor, 1, axis1, m1.stepNum);        
              ButtonDisplayTouch(1);        
              break;
      
            case CH2:  //motor 2
              Serial.println("------------------MOTOR 2:");
              d1.screenOn(LabelsX, 2, axis2, m1.stepNum);
              ButtonDisplayTouch(2);        
              break;
      
            case CH3:  //motor 3
              Serial.println("------------------MOTOR 3:");
              d1.screenOn(LabelsY, 3, axis3, m1.stepNum);
              ButtonDisplayTouch(3);         
              break; 
          }
            
          if(digitalRead(ST)==false){
            UserInterface=1;
            Serial.println("DISCONECTED Bluetooth DEVICE");
            d1.RemoteControl(4);
            delay(1000);
            screen=1;
            d1.MainScreen1(axis1, axis2, axis3);            
          }

        }        
        break;      
    }
    
  }else{
    //Send warning to RAControl if it's into the system
    

  }
}


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//////////////////  [Keyboard control] //////////////////////////////
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void ButtonKeyboard(int motor){

  screen=2;
  int auxMotor=motor;
  while (auxMotor==motor) {
    //RAControl has priority
    if(digitalRead(ST)==true){
      UserInterface=3;
      Serial.println("CONNECTED Bluetooth DEVICE");
      d1.RemoteControl(3);
      delay(500);            
      screen=1;
      d1.MainScreen1(axis1, axis2, axis3);     
      motor=0;       
    }else{
      usb.Task();
      if (keyPress!=0) {
        switch (keyPress) {
          case CH1:  //motor 1
            /***** Select motor 1 *****/
            motor=1;
            break;
                  
          case CH2:  //motor 2
            /***** Select motor 2 *****/
            motor=2;
            break;
      
          case CH3:  //motor 3
            /***** Select motor 3 *****/
            motor=3;
            break;
      
          case Down_Forward_Left:
            /***** Down *****/
            // color1.White();
            if(motor==1){                                 
              axis1 = m1.down(axis1, false);
              d1.setStep(axis1);
              s1.WriteFLOAT(1, axis1);
            }else if(motor==2){
              axis2 = m1.forward(axis2, false);
              d1.setStep(axis2);
              s1.WriteFLOAT(2, axis2);
            }else if(motor==3){
              axis3 = m1.left(axis3, false);
              d1.setStep(axis3 );
              s1.WriteFLOAT(3, axis3);
            }
            // color1.Blue();
            break;
          
          case Up_Backward_Right:  
            /***** Up *****/
            // color1.White();
            if(motor==1){                      
              axis1 = m1.up(axis1, false);
              d1.setStep(axis1); 
              s1.WriteFLOAT(1, axis1);
            }else if(motor==2){
              axis2 = m1.backward(axis2, false);
              d1.setStep(axis2);
              s1.WriteFLOAT(2, axis2);
            }else if(motor==3){
              axis3 = m1.right(axis3, false);
              d1.setStep(axis3);
              s1.WriteFLOAT(3, axis3);
            }
            // color1.Blue();
            break;
      
          case SLOW:  //VOL- to short step
            //***** Short *****
            if(m1.stepNum!=1){
              m1.stepNum = m1.Short(m1.stepNum);
              d1.stepLength(m1.stepNum);
            }
            break;
            
          case FAST:  //VOL+ to long step
            //***** Long *****
            if(m1.stepNum!=3){
              m1.stepNum = m1.Long(m1.stepNum);
              d1.stepLength(m1.stepNum);
            }
            break;
      
          case RESET:  //100+ to Reset Position
            /***** Reset position *****/
            if(motor==1){
              axis1=m1.ResetPositionMotorKeyboard(d1, motor, axis1, usb, keyboard, keyPress, keyRelease);
              s1.WriteFLOAT(1, axis1);
            }else if(motor==2){
              axis2=m1.ResetPositionMotorKeyboard(d1, motor, axis2, usb, keyboard, keyPress, keyRelease);
              s1.WriteFLOAT(2, axis2);
            }else if(motor==3){
              axis3=m1.ResetPositionMotorKeyboard(d1, motor, axis3, usb, keyboard, keyPress, keyRelease);
              s1.WriteFLOAT(3, axis3);
            }
            break;

          case REBOOT:
            /***** Reboot *****/
            keyPress=0; 
            d1.AskReboot();
            aux=1;
            while(aux == 1) {
              usb.Task();
              if (keyPress == ENTER) {
                Reboot_y_n(keyPress, motor);
                aux=0;
              }else if (keyPress == EXIT) {             
                Reboot_y_n(keyPress, motor);
                aux=0;
              }
              keyPress=0;
            }
            // color1.Blue();
            break;
            
          case EXIT:  //to Exit, go to Main screen
            /***** Exit *****/
            d1.MainScreen1(axis1, axis2, axis3);
            motor=0;
            break;           
        }
        keyPress=0;
      }
    }
  }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//////////////////  [Remote control]  ///////////////////////////////
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void ButtonRemoteControl(int motor){
  int auxMotor=motor;
  while (auxMotor==motor) {
    //RAControl has priority
    if(digitalRead(ST)==true){
      UserInterface=3;
      Serial.println("CONNECTED Bluetooth DEVICE");
      d1.RemoteControl(3);
      delay(500);            
      screen=1;
      d1.MainScreen1(axis1, axis2, axis3);     
      motor=0;       
    }else{    

      if (irrecv.decode(&results)) {  

        if(results.value == Repeat_) { // if the value is equal to 0xFFFFFFFF
          results.value = key_value; // set the value to the key value
          if(results.value == SLOW or results.value == FAST) {
            delay(500);
          }
        }      

        //Serial.println(String("value: ") + results.value);

        switch (results.value) {
          case CH1:  //motor 1
            /***** Select motor 1 *****/
            motor=1;
            break;
                  
          case CH2:  //motor 2
            /***** Select motor 2 *****/
            motor=2;
            break;
      
          case CH3:  //motor 3
            /***** Select motor 3 *****/
            motor=3;
            break;
      
          case Down_Forward_Left:
            if(motor==1){            
              axis1 = m1.down(axis1, false);            
              s1.WriteFLOAT(1, axis1);  
              d1.setStep(axis1);            
            }else if(motor==2){
              axis2 = m1.forward(axis2, false);  
              s1.WriteFLOAT(2, axis2); 
              d1.setStep(axis2); 
            }else if(motor==3){
              axis3 = m1.left(axis3, false);    
              s1.WriteFLOAT(3, axis3); 
              d1.setStep(axis3 ); 
            }
            break;
      
          case Up_Backward_Right:  
            /***** Up *****/
            if(motor==1){
              axis1 = m1.up(axis1, false);
              d1.setStep(axis1);
              s1.WriteFLOAT(1, axis1);
            }else if(motor==2){
              axis2 = m1.backward(axis2, false);
              d1.setStep(axis2);
              s1.WriteFLOAT(2, axis2);
            }else if(motor==3){
              axis3 = m1.right(axis3, false);
              d1.setStep(axis3);
              s1.WriteFLOAT(3, axis3);
            }
            break;
      
          case SLOW:  //VOL- to short step
            //***** Short *****
            if(m1.stepNum!=1){
              m1.stepNum = m1.Short(m1.stepNum);
              d1.stepLength(m1.stepNum);
            }
            break;
            
          case FAST:  //VOL+ to long step
            //***** Long *****
            if(m1.stepNum!=3){
              m1.stepNum = m1.Long(m1.stepNum);
              d1.stepLength(m1.stepNum);
            }
            break;
      
          case RESET:  //100+ to Reset Position
            /***** Reset position *****/
            if(motor==1){
              axis1=m1.ResetPositionMotorRemoteControl(d1, motor, axis1, irrecv, results);
              s1.WriteFLOAT(1, axis1);
            }else if(motor==2){
              axis2=m1.ResetPositionMotorRemoteControl(d1, motor, axis2, irrecv, results);
              s1.WriteFLOAT(2, axis2);
            }else if(motor==3){
              axis3=m1.ResetPositionMotorRemoteControl(d1, motor, axis3, irrecv, results);
              s1.WriteFLOAT(3, axis3);
            }
            break;

          case REBOOT:
          /***** Reboot *****/  
            d1.AskReboot();
            aux=1;
            while(aux == 1) {
              if (irrecv.decode(&results)) {
                if(results.value == ENTER){
                  Reboot_y_n(results.value, motor);
                  aux=0;
                }else if(results.value == EXIT){
                  Reboot_y_n(results.value, motor);
                  aux=0;
                }
                irrecv.resume(); // reset the receiver for the next code
              }            
            }
            break;
            
          case EXIT:  //to Exit, go to Main screen
            /***** Exit *****/
            d1.MainScreen1(axis1, axis2, axis3);
            motor=0;
            break;
        }
        key_value = results.value;
        irrecv.resume(); // reset the receiver for the next code
      }
    }
  }
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//////////////////  [ButtonDisplayTouch] ////////////////////////////
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void ButtonDisplayTouch(int motor){

  screen=2;
  int auxMotor=motor;
  responseAT="";
  while (auxMotor==motor) {
    //RAControl has priority
    while(Serial3.available()>0){
      a = char(Serial3.read());
      responseAT += a;
    }    
    if(responseAT.indexOf(".")!=-1){
      Serial.println(responseAT);
      responseAT=responseAT.substring(0, responseAT.length()-1); //remove \r\n from command string  
      touch=responseAT.toInt();
      Serial.println(touch);
      switch (touch) {
        case CH1:  //motor 1
          /***** Select motor 1 *****/
          Serial.println("es motor 1");
          motor=1;
          break;
                
        case CH2:  //motor 2
          /***** Select motor 2 *****/
          Serial.println("es motor 2");
          motor=2;
          break;
    
        case CH3:  //motor 3
          /***** Select motor 3 *****/
          Serial.println("es motor 3");
          motor=3;
          break;
    
        case Down_Forward_Left:
          /***** Down *****/
          // color1.White();
          Serial.println("es down");
          if(motor==1){                                 
            axis1 = m1.down(axis1, false);
            d1.setStep(axis1);
            s1.WriteFLOAT(1, axis1);
          }else if(motor==2){
            axis2 = m1.forward(axis2, false);
            d1.setStep(axis2);
            s1.WriteFLOAT(2, axis2);
          }else if(motor==3){
            axis3 = m1.left(axis3, false);
            d1.setStep(axis3 );
            s1.WriteFLOAT(3, axis3);
          }
          // color1.Blue();
          break;
        
        case Up_Backward_Right:  
          /***** Up *****/
          // color1.White();
          Serial.println("es up");
          if(motor==1){                      
            axis1 = m1.up(axis1, false);
            d1.setStep(axis1); 
            s1.WriteFLOAT(1, axis1);
          }else if(motor==2){
            axis2 = m1.backward(axis2, false);
            d1.setStep(axis2);
            s1.WriteFLOAT(2, axis2);
          }else if(motor==3){
            axis3 = m1.right(axis3, false);
            d1.setStep(axis3);
            s1.WriteFLOAT(3, axis3);
          }
          // color1.Blue();
          break;
    
        case SLOW:  //VOL- to short step
          //***** Short *****
          Serial.println("es slow");
          if(m1.stepNum!=1){
            m1.stepNum = m1.Short(m1.stepNum);
            d1.stepLength(m1.stepNum);
          }
          break;
          
        case FAST:  //VOL+ to long step
          //***** Long *****
          Serial.println("es fast");
          if(m1.stepNum!=3){
            m1.stepNum = m1.Long(m1.stepNum);
            d1.stepLength(m1.stepNum);
          }
          break;

        case REBOOT:////////////////////////////////////////////////WORKING ON
          /***** Reboot *****/
          Serial.println("es reboot");
          keyPress=0; 
          d1.AskReboot();
          aux=1;
          while(aux == 1) {
            usb.Task();
            if (keyPress == ENTER) {
              Reboot_y_n(keyPress, motor);
              aux=0;
            }else if (keyPress == EXIT) {             
              Reboot_y_n(keyPress, motor);
              aux=0;
            }
            keyPress=0;
          }
          // color1.Blue();
          break;
          
        case EXIT:  //to Exit, go to Main screen
          /***** Exit *****/
          Serial.println("es exit");
          d1.MainScreen1(axis1, axis2, axis3);
          motor=0;
          break;           
      }
      responseAT="";
    }
  }
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////    DETECT SIGNAL    ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

bool detectSignal(int s){

  a1 = s1.ReadFLOAT(1);
  a2 = s1.ReadFLOAT(2);
  a3 = s1.ReadFLOAT(3);
  Serial.print("Axis 1= ");
  Serial.println(a1);
  Serial.print("Axis 2= ");  
  Serial.println(a2);
  Serial.print("Axis 3= ");  
  Serial.println(a3);

  d1.SPA_System();

  if(a1 == float(257.55) or a2 == float(257.55) or a3 == float(257.55)){    
    d1.LostSignal(); //can't communicate with EEPROM memory
    return false;
  }else{
    if(s == 0){
      axis1 = a1;
      axis2 = a2;
      axis3 = a3;
      d1.MainScreen1(axis1, axis2, axis3);
      //motors ON
      //RedButtonValue = analogRead(RedButton);      
      m1.start();
    }    
    return true;
  }
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////    REBOOT YES OR NO    ////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
void Reboot_y_n(unsigned long answer, int motor){

  switch (answer) {
    case ENTER:  //to do rebooting
        /***** Enter *****/
        d1.Reboot_HomePosition();
        switch (motor){
          case 1:
              //**go to home position
              while(digitalRead(SWITCH_M1_1)==true){
                axis1 = m1.down(axis1, true);              
              }
      
              //**take the last position saved from the storage
              last = s1.ReadFLOAT(motor);
              
              //**go to the last position saved        
              temp=0;
              d1.Reboot_LastPosition(last, temp);          
              while(temp < last){          
                temp = m1.up(temp, true); 
                d1.rebootPosition(temp);
              }
      
              axis1=last;
              d1.successful(2);
              d1.screenOn(Conveyor, 1, axis1, m1.stepNum);
            
            break;
            
          case 2:
              //**go to position 0            
              while(digitalRead(SWITCH_M2_4)==true){
                //axis2 = m1.forward(axis2, true);    
                axis2 = m1.backward(axis2, true);              
              }
      
              //**take the last position saved from the storage
              last = s1.ReadFLOAT(motor);
              
              //**go to the last position saved        
              temp=0;
              d1.Reboot_LastPosition(last, temp);  
              while(temp < last){
                //temp = m1.backward(temp, true); 
                temp = m1.forward(temp, true); 
                d1.rebootPosition(temp);
              }
              axis2=last;
              d1.successful(2);
              d1.screenOn(LabelsX, 2, axis2, m1.stepNum);
            
            break;
            
          case 3:
              //**go to position 0            
              while(digitalRead(SWITCH_M3_5)==true){
                axis3 = m1.right(axis3, true);
              }
      
              //**take the last position saved from the storage
              last = s1.ReadFLOAT(motor);
              
              //**go to the last position saved        
              temp=0;
              d1.Reboot_LastPosition(last, temp);  
              while(temp < last){
                temp = m1.left(temp, true); 
                d1.rebootPosition(temp);
              }
              
              axis3=last;
              d1.successful(2);
              d1.screenOn(LabelsY, 3, axis3, m1.stepNum);        
            break;
        }
      break;

    case EXIT:  //to go back
        /***** back *****/
        switch (motor){
          case 1:
              d1.screenOn(Conveyor, 1, axis1, m1.stepNum); 
            break;
            
          case 2:
              d1.screenOn(LabelsX, 2, axis2, m1.stepNum);
            break;
            
          case 3:
              d1.screenOn(LabelsY, 3, axis3, m1.stepNum);
            break;
        }
      break;
  }

}
