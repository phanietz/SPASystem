#include "Display.h"
#include "StatusIndicators.h"
#include "IRremote2.h" //to use IR controller remote in this class
#include <KeyboardController.h> //to use Keyboard in this class
#include "Wire.h"  //use i2c
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header
hd44780_I2Cexp lcd; // declare lcd object: auto locate & auto config expander chip
Colors color;
int * control = &color.control;

#define CH1 16753245 //To move motor 1, sensor of conveyor
#define CH2 16736925 //To move motor 2, sensor of the labels X axis
#define CH3 16769565 //To move motor 3, sensor of the labels Y axis
#define Down_Backward_Right 16720605 //To move Down or right or backward
#define Up_Forward_Left 16712445 //To move Up or left or forward
#define ENTER 16761405 //Enter
#define SLOW 16769055 //Slow  //NEXT to go Right
#define FAST 16754775 //Fast
#define EXIT 16748655 //Exit
#define RESET 16750695 //To Reset position
#define REBOOT 16756815 //To reboot
#define IR_remote 16738455 //To activate IR remote control
#define Repeat_ 4294967295 


Display::Display(){ //Constructor
        /*
        this->sensor = sensor;
        this->motor = motor;
        this->gotostep = gotostep;         
        */        
}

void Display::screenOn(String title, int motor, float axis, int stepNum){
  
  status = lcd.begin(LCD_COLS, LCD_ROWS);
  if(status){ // non zero status means it was unsuccesful
    // hd44780 has a fatalError() routine that blinks an led if possible
    // begin() failed so blink error code using the onboard LED if possible
    hd44780::fatalError(status); // does not return
  }
  
  //*******Turn ON Display*********
  //lcd.init();     //initialize the lcd
  //lcd.display();
  //lcd.backlight();

  MainScreen2(title,motor,axis,stepNum);
}

void Display::SPA_System(){

  status = lcd.begin(LCD_COLS, LCD_ROWS);
  if(status){ // non zero status means it was unsuccesful
    // hd44780 has a fatalError() routine that blinks an led if possible
    // begin() failed so blink error code using the onboard LED if possible
    hd44780::fatalError(status); // does not return
  }
/*
  lcd.setCursor(0, 0); 
  lcd.print("       SENSOR       "); //0
  lcd.setCursor(0, 1);
  lcd.print("      POSITION      "); //1
  lcd.setCursor(0, 2);
  lcd.print("     ADJUSTMENT     "); //2  
  lcd.setCursor(0, 3);
  lcd.print("       SYSTEM       "); //3  
  */
/*
  lcd.setCursor(0, 0); 
  lcd.print("  SENSOR POSITION   "); //0
  lcd.setCursor(0, 1);
  lcd.print("     ADJUSTMENT     "); //1  
  lcd.setCursor(0, 2);
  lcd.print("       SYSTEM       "); //2
  lcd.setCursor(0, 3);
  lcd.print("                    "); //3   
*/

  //color.TestColor();
  color.WeatherTech();  
  
  lcd.setCursor(0, 0); 
  lcd.print("                    "); //0
  lcd.setCursor(0, 1);
  lcd.print("  SENSOR POSITION   "); //1  
  lcd.setCursor(0, 2);
  lcd.print(" ADJUSTMENT SYSTEM  "); //2
  lcd.setCursor(0, 3);
  lcd.print("                    "); //3   
  
  
  delay(1500);
}

void Display::MainScreen1(float axis1, float axis2, float axis3){
  //lcd.print("     SPA SYSTEM     "); //0
  //lcd.print("  Motor 1:  25.50   "); //1
  //lcd.print("  Motor 2:  16.75   "); //2
  //lcd.print("  Motor 3:   8.00   "); //3
  
  //lcd.init();     //initialize the lcd
  //lcd.display();
  //lcd.backlight();
  
  status = lcd.begin(LCD_COLS, LCD_ROWS);
  if(status){ // non zero status means it was unsuccesful
    // hd44780 has a fatalError() routine that blinks an led if possible
    // begin() failed so blink error code using the onboard LED if possible
    hd44780::fatalError(status); // does not return
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  SENSOR POSITION  "); //0
  //lcd.print("     SPA SYSTEM     "); //0
  
  lcd.setCursor(0, 1);  
  if (axis1 <10){
    lcd.print(String(" 1 Conveyor z:  ") + String(axis1)); //1
  }else{
    lcd.print(String(" 1 Conveyor z: ") + String(axis1));  //1  
  }

  lcd.setCursor(0, 2);
  if (axis2 <10){
    lcd.print(String(" 2 Label    x:  ") + String(axis2)); //2
  }else{
    lcd.print(String(" 2 Label    x: ") + String(axis2));  //2  
  }  

  lcd.setCursor(0, 3);
  if (axis3 <10){
    lcd.print(String(" 3 Label    y:  ") + String(axis3)); //3
  }else{
    lcd.print(String(" 3 Label    y: ") + String(axis3));  //3  
  }  

  color.PushButtom();
  color.Control();

  
}

void Display::MainScreen2(String title, int motor, float axis, int stepNum){
  //lcd.print("   SENSOR LABELS    "); //0
  
  //lcd.print("  SENSOR CONVEYOR   "); //0
  //lcd.print(" Motor: 1           "); //1
  //lcd.print(" Position: -45.25   "); //2
  //lcd.print(" Step: |L|  M   H   "); //3   
  
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(String(title)); //0
  lcd.setCursor(0, 1);
  lcd.print(String("  Motor: ") + String(motor)); //1
  lcd.setCursor(0, 2);
  lcd.print(String("  Position:")); //2
  setStep(axis); 
  stepLength(stepNum); //3  
}

void Display::setStep(float axis){
  //Write Axis on the 2do line
  //lcd.setCursor(11, 2);
  lcd.setCursor(12, 2); //====================================
  if (axis <10){
    lcd.print(String(" ") + String(axis));
  }else{
    lcd.print(String(axis));    
  }
 
}


void Display::stepLength(int stepNum){
  //lcd.print(" Step: |L|  M   H   "); //3
  //lcd.print(" Step:  L  |M|  H   "); //3
  //lcd.print(" Step:  L   M  |H|  "); //3
    
  lcd.setCursor(0, 3);  

  if(stepNum==1){      
    lcd.print("  Step: |L|  M   H   ");
  }else if(stepNum==2){    
    lcd.print("  Step:  L  |M|  H   ");
  }else if(stepNum==3){
    lcd.print("  Step:  L   M  |H|  ");
  }

  color.PushButtom();
  color.Control();

}
////////////////////////////////////////////
////////////////////////////////////////////
////////////////////////////////////////////
////////////////////////////////////////////
void Display::reset(int motor, float axis){ 
  //lcd.print("       RESET        "); //0
  //lcd.print("  INITIAL POSITION  "); //1
  //lcd.print("     FOR MOTOR 1    "); //2
  //lcd.print("    Axis: -25.50    "); //3
  
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("       RESET        "); //0
  lcd.setCursor(0, 1);
  lcd.print("  INITIAL POSITION  "); //1
  lcd.setCursor(0, 2);
  lcd.print(String("     FOR MOTOR ") + String(motor)); //2
  lcd.setCursor(0, 3);
  lcd.print(String("   Axis:")); //3  

  resetPosition(axis);
  
}

void Display::resetPosition(float axis){
  //Write Axis
  lcd.setCursor(10, 3);   
  
  if(axis >= 10.00){
    lcd.print(String(" ") + String(axis));    
  }else if (axis >= 0.00 and axis <= 9.75){
    lcd.print(String("  ") + String(axis));
  }else if(axis >= -9.75 and axis <= 0.00){
    lcd.print(String(" ") + String(axis));
  }else{
    lcd.print(String(axis));
  }

  color.PushButtom();
  color.Info();   
  //color.Control();   
}


void Display::AskareYouSure(){
  //lcd.print("                    "); //0
  //lcd.print("    Are you sure    "); //1
  //lcd.print("   reset axis 0 ?   "); //2
  //lcd.print("                    "); //3
 
  lcd.clear();
  
  lcd.setCursor(0, 0);  
  lcd.print("                    "); //0
  lcd.setCursor(0, 1);  
  lcd.print("    Are you sure    "); //1
  lcd.setCursor(0, 2);  
  lcd.print("   reset axis 0 ?   "); //2
  lcd.setCursor(0, 3);  
  lcd.print("                    "); //3  
  
  color.Question();
}


void Display::AskgoBack(){
  //lcd.print("                    "); //0
  //lcd.print("   Do you want to   "); //1
  //lcd.print("       exit ?       "); //2
  //lcd.print("                    "); //3  
    
  lcd.clear();

  lcd.setCursor(0, 0); 
  lcd.print("                    "); //0
  lcd.setCursor(0, 1);
  lcd.print("   Do you want to   "); //1
  lcd.setCursor(0, 2);
  lcd.print("       exit ?       "); //2
  lcd.setCursor(0, 3);
  lcd.print("                    "); //3 
  
  color.Question();
}

void Display::donotSaveChangesMESSAGE(){
  //lcd.print("      Waiting       "); //0
  //lcd.print("    for the motor   "); //1
  //lcd.print("  to finish moving  "); //2
  //lcd.print("                    "); //3
    
  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print("      Waiting       "); //0
  lcd.setCursor(0, 1);
  lcd.print("    for the motor   "); //1
  lcd.setCursor(0, 2);
  lcd.print("  to finish moving  "); //2
  lcd.setCursor(0, 3);
  lcd.print("                    "); //3

  color.Warning();
  delay(1000);
}

void Display::returning(float axis){
  //Write Axis
  lcd.setCursor(6, 3);

  if(axis >= 10.00){
    lcd.print(String(" ") + String(axis));    
  }else if (axis >= 0.00 and axis <= 9.75){
    lcd.print(String("  ") + String(axis));
  }else if(axis >= -9.75 and axis <= 0.00){
    lcd.print(String(" ") + String(axis));
  }else{
    lcd.print(String(axis));
  }

}

void Display::successful(int message){
  //lcd.print("                    "); //0
  //lcd.print("   New position 0   "); //1
  //lcd.print("     successful     "); //2
  //lcd.print("                    "); //3     
                      //OR
  //lcd.print("                    "); //0
  //lcd.print("     Rebooting      "); //1
  //lcd.print("     successful     "); //2
  //lcd.print("                    "); //3      
  
  lcd.clear();
  if (message==1){
    //Reset
    lcd.setCursor(0, 0);
    lcd.print("                    "); //0
    lcd.setCursor(0, 1);
    lcd.print("   New position 0   "); //1
    lcd.setCursor(0, 2);
    lcd.print("     successful     "); //2
    lcd.setCursor(0, 3);
    lcd.print("                    "); //

  }else if (message==2){
    //Reboot
    lcd.setCursor(0, 0);
    lcd.print("                    "); //0
    lcd.setCursor(0, 1);
    lcd.print("     Rebooting      "); //1
    lcd.setCursor(0, 2);
    lcd.print("     successful     "); //2
    lcd.setCursor(0, 3);
    lcd.print("                    "); //3  
  }
 
  color.Successful();

}

////////////////////////////////////////////
////////////////////////////////////////////
void Display::AskReboot(){
  //lcd.print("                    "); //0
  //lcd.print("   Do you want to   "); //1
  //lcd.print("       reboot?      "); //2
  //lcd.print("                    "); //3

  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print("                    "); //0
  lcd.setCursor(0, 1);
  lcd.print("   Do you want to   "); //1
  lcd.setCursor(0, 2);
  lcd.print("       reboot?      "); //2
  lcd.setCursor(0, 3);
  lcd.print("                    "); //3

  color.Question();
}

void Display::Reboot_HomePosition(){
  //lcd.print("                    "); //0
  //lcd.print("      Moving to     "); //1
  //lcd.print("    home position   "); //2
  //lcd.print("                    "); //3

  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print("                    "); //0
  lcd.setCursor(0, 1);
  lcd.print("      Moving to     "); //1
  lcd.setCursor(0, 2);
  lcd.print("    home position   "); //2
  lcd.setCursor(0, 3);
  lcd.print("                    "); //3

  color.Warning();
}

void Display::Reboot_LastPosition(float last, float axis){
  //lcd.print("      Moving to     "); //0
  //lcd.print("     last saved     "); //1
  //lcd.print("      position      "); //2
  //lcd.print("   xx.xx    xx.xx   "); //3
    
  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print("      Moving to     "); //0
  lcd.setCursor(0, 1);
  lcd.print("     last saved     "); //1
  lcd.setCursor(0, 2);
  lcd.print("      position      "); //2  
  lcd.setCursor(0, 3);
  lcd.print(String("            ") + String(last)); //3
  
  rebootPosition(axis);
  color.ChangeMoving();
}

void Display::rebootPosition(float axis){
  //Write Axis
  lcd.setCursor(3, 3);
  
  if(axis >= 10.00){
    lcd.print(String(axis));    
  }else{
    lcd.print(String(" ") + String(axis));
  }
  
}

//////////////////////////////////////////////
//////////////////////////////////////////////
void Display::LostSignal(){
  //lcd.print("    LOST SIGNAL     "); //0
  //lcd.print(" Controller failed  "); //1
  //lcd.print("to communicate with "); //2
  //lcd.print("   memory module    "); //3
  status = lcd.begin(LCD_COLS, LCD_ROWS);
  if(status){ // non zero status means it was unsuccesful
    // hd44780 has a fatalError() routine that blinks an led if possible
    // begin() failed so blink error code using the onboard LED if possible
    hd44780::fatalError(status); // does not return
  }
  
  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print("    LOST SIGNAL     "); //0
  lcd.setCursor(0, 1);
  lcd.print(" Controller failed  "); //1
  lcd.setCursor(0, 2);
  lcd.print("to communicate with "); //2
  lcd.setCursor(0, 3);
  lcd.print("   memory module    "); //3  

  color.Error();
  
}


bool Display::AskActivateRemoteControl(class USBHost &usb2, unsigned long &keyPress){
  //lcd.print("   Do you want to   "); //0
  //lcd.print("    activate the    "); //1
  //lcd.print("  remote control?   "); //2
  //lcd.print("                    "); //3

  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print("   Do you want to   "); //0
  lcd.setCursor(0, 1);
  lcd.print("    activate the    "); //1
  lcd.setCursor(0, 2);
  lcd.print("  remote control?   "); //2
  lcd.setCursor(0, 3);
  lcd.print("                    "); //3    

  color.Question();
  
  keyPress=0;
  while (true) {    
    usb2.Task();
    if (keyPress!=0) {
      switch (keyPress) { 
        case ENTER:  //PLAY to confirm exit and do not save changes
          /***** ACTIVATED REMOTE CONTROL*****/     
          RemoteControl(1);            
          color.control = 2;
          return true;
          break;
        case EXIT:  //EQ to ignore, to back (go to screen reset position)
          /***** Not activated *****/
          color.control = 1;
          return false;
          break;
      }
    }
    keyPress=0;    
  } 
}

bool Display::AskDeactivateRemoteControl(class USBHost &usb2, unsigned long &keyPress){
  //lcd.print("   Do you want to   "); //0
  //lcd.print("   deactivate the   "); //1
  //lcd.print("  remote control?   "); //2
  //lcd.print("                    "); //3

  lcd.clear();
  lcd.setCursor(0, 0); 
  lcd.print("   Do you want to   "); //0
  lcd.setCursor(0, 1);
  lcd.print("   deactivate the   "); //1
  lcd.setCursor(0, 2);
  lcd.print("  remote control?   "); //2
  lcd.setCursor(0, 3);
  lcd.print("                    "); //3    

  color.Question();
  
  keyPress=0;
  while (true) {    
    usb2.Task();
    if (keyPress!=0) {
      switch (keyPress) {     
        case ENTER:  //PLAY to confirm exit and do not save changes
          //***** DEACTIVATED REMOTE CONTROL *****
          color.control = 1;
          RemoteControl(2);            
          return false;
          break;
        case EXIT:  //EQ to ignore, to back (go to screen reset position)
          //***** Not deactivated *****
          color.control = 2;
          return true;
          break;
      }
    }
    keyPress=0;    
  } 
}

void Display::RemoteControl(int state){
  //lcd.print("                    "); //0
  //lcd.print("   Remote control   "); //1
  //lcd.print("     activated      "); //2
  //lcd.print("                    "); //3
    
                  //OR
    
  //lcd.print("                    "); //0
  //lcd.print("   Remote control   "); //1
  //lcd.print("    deactivated     "); //2
  //lcd.print("                    "); //3
    
  lcd.clear();

  if(state==1){
    lcd.setCursor(0, 0); 
    lcd.print("                    "); //0
    lcd.setCursor(0, 1);
    lcd.print("   Remote control   "); //1
    lcd.setCursor(0, 2);
    lcd.print("     activated      "); //2
    lcd.setCursor(0, 3);
    lcd.print("                    "); //3
    color.Successful();
    delay(500);
  }else if(state==2){ //go back to keyboard control
    lcd.setCursor(0, 0); 
    lcd.print("                    "); //0
    lcd.setCursor(0, 1);
    lcd.print("   Remote control   "); //1
    lcd.setCursor(0, 2);
    lcd.print("    deactivated     "); //2
    lcd.setCursor(0, 3);
    lcd.print("                    "); //3   
    color.Successful();
    delay(500); 
  }else if(state==3){
    lcd.setCursor(0, 0); 
    lcd.print("                    "); //0
    lcd.setCursor(0, 1);
    lcd.print("      RAControl     "); //1
    lcd.setCursor(0, 2);
    lcd.print("      connected     "); //2
    lcd.setCursor(0, 3);
    lcd.print("                    "); //3    
    color.Successful(); 
    color.control = 3;

  }else if(state==4){ //go back to keyboard control
    lcd.setCursor(0, 0); 
    lcd.print("                    "); //0
    lcd.setCursor(0, 1);
    lcd.print("      RAControl     "); //1
    lcd.setCursor(0, 2);
    lcd.print("    disconnected    "); //2
    lcd.setCursor(0, 3);
    lcd.print("                    "); //3
    color.Info(); 
    color.control = 1;
  }

}
