#include "Display.h"
#include "IRremote2.h" //to use IR controller remote in this class
#include <KeyboardController.h> //to use Keyboard in this class
#include "Motors.h"
#include "Stepper.h"
#include "StatusIndicators.h" //to get acces control variable
#define CH1 16753245 //To move motor 1, sensor of conveyor
#define CH2 16736925 //To move motor 2, sensor of the labels X axis
#define CH3 16769565 //To move motor 3, sensor of the labels Y axis

//#define Down_Backward_Right 16720605 //To move Down or right or backward
//#define Up_Forward_Left 16712445 //To move Up or left or forward
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

/*----- Global Variables and Pins ------*/
#define STEPS  32   // Number of steps per revolution of Internal shaft
//Setup of proper sequencing for MotSteps2Takeor Driver Pins
Stepper small_stepper1(STEPS, 22, 24, 23, 25); // In1, In2, In3, In4 in the sequence 1-3-2-4
Stepper small_stepper2(STEPS, 26, 28, 27, 29); // In1, In2, In3, In4 in the sequence 1-3-2-4
Stepper small_stepper3(STEPS, 30, 32, 31, 33); // In1, In2, In3, In4 in the sequence 1-3-2-4
Colors color2;

Motors::Motors(){
  //small_stepper1.step(1);
  //small_stepper1.step(-1);
}

void Motors::start(){
  small_stepper1.setSpeed(700);
  small_stepper1.step(1);
  small_stepper1.step(-1);
  small_stepper2.setSpeed(700);
  small_stepper2.step(1);
  small_stepper2.step(-1);  
  small_stepper3.setSpeed(700);
  small_stepper3.step(1);
  small_stepper3.step(-1);  
}
/////////////////////////////////////////////////////////////////////
////////////////////  MOTOR 1  //////////////////////////////////////
///   Motor 1 uses the methods up and down  
///   Method down is to go to position 0    
/////////////////////////////////////////////////////////////////////
float Motors::up(float CountSteps, bool unblocked){
  //digitalRead(40)
  //if(CountSteps == num_steps_UP and unblocked == false){
  if(digitalRead(41) == false and unblocked == false){
    Serial.println("It's 42 now");                        
  }else{
    aux=CountSteps+lenght;
    if(aux<=num_steps_UP){
    //if(digitalRead(40) == true){
      small_stepper1.setSpeed(700);      
      small_stepper1.step(-int(longStep));  
      //small_stepper1.setSpeed(0);
      CountSteps=aux;
    }else{
      Serial.println("limit");  
    }
  }
  Serial.println(CountSteps);
  return CountSteps;
}

float Motors::down(float CountSteps, bool unblocked){
  //if(CountSteps == 0 and unblocked == false){
  if(digitalRead(40) == false and unblocked == false){
    Serial.println("It's on position 0");
  }else{
    aux=CountSteps-lenght;
    if(aux>=0 or unblocked == true){
    //if(digitalRead(40) == true or unblocked == true){
      small_stepper1.setSpeed(700);
      small_stepper1.step(int(longStep));
      //small_stepper1.setSpeed(0);
      CountSteps=aux;      
    }
  }
  Serial.println(CountSteps);
  return CountSteps;
}

/////////////////////////////////////////////////////////////////////
////////////////////  MOTOR 2  //////////////////////////////////////
///   Motor 2 uses the methods forward and backward
///   Method backward is to go to position 0
/////////////////////////////////////////////////////////////////////

float Motors::backward(float CountSteps, bool unblocked){
  if(digitalRead(43) == false and unblocked == false){
    Serial.println("It's on position 0");                        
  }else{ 
    aux=CountSteps-lenght;
    if(aux>=0 or unblocked == true){
    //if(digitalRead(43) == true){
      small_stepper2.setSpeed(700); //Max seems to be 700    
      small_stepper2.step(int(longStep));
      CountSteps=aux;
    }
  }
  Serial.println(CountSteps);
  return CountSteps;  
}

float Motors::forward(float CountSteps, bool unblocked){
    //if(CountSteps == 0 and unblocked == false){
    if(digitalRead(42) == false and unblocked == false){
      Serial.println("It's 14 now");
    }else{
      aux=CountSteps+lenght;
      if(aux<=num_steps_FORWARD){
      // if(digitalRead(42) == true or unblocked == true){            
        small_stepper2.setSpeed(700);
        small_stepper2.step(-int(longStep));
        CountSteps=aux;
      }else{
      Serial.println("limit");  
      }
    }
    Serial.println(CountSteps);
    return CountSteps;    
}

/////////////////////////////////////////////////////////////////////
////////////////////  MOTOR 3  //////////////////////////////////////
///   Motor 3 uses the methods left and right
///   Method right is to go to position 0
/////////////////////////////////////////////////////////////////////

float Motors::right(float CountSteps, bool unblocked){    
    //if(CountSteps == num_steps_LEFT and unblocked == false){
    if(digitalRead(44) == false and unblocked == false){
      Serial.println("It's on position 0");
    }else{
    aux=CountSteps-lenght;
    if(aux>=0 or unblocked == true){
    //if(digitalRead(45) == true){      
        small_stepper3.setSpeed(700);
        small_stepper3.step(-int(longStep));
        CountSteps=aux;        
      }
    }
    Serial.println(CountSteps);
    return CountSteps;
}

float Motors::left(float CountSteps, bool unblocked){
  //if(CountSteps == 0 and unblocked == false){
  if(digitalRead(45) == false and unblocked == false){
    Serial.println("It's 16 now");                        
  }else{ 
    aux=CountSteps+lenght;
    if(aux<=num_steps_LEFT){
    //if(digitalRead(44) == true or unblocked == true){    
      small_stepper3.setSpeed(700); //Max seems to be 700    
      small_stepper3.step(int(longStep));
      CountSteps=aux;
    }else{
      Serial.println("limit");  
    }
  }
  Serial.println(CountSteps);
  return CountSteps;
}


/////////////////////////////////////////////////////////////////////
////////////////////  SIZE STEP  ////////////////////////////////////
///   SPASystem uses 3 kind of steps: L= 0.25   M= 0.50   H= 1.0
///   **lenght M is default step when the system turned on
///   **Reset Potion always going to use L lenght step
/////////////////////////////////////////////////////////////////////
int Motors::Short(int stepNum){      
  if(stepNum>1){
    stepNum=stepNum-1;
    stepLength(stepNum);
  }
  return stepNum;
}


int Motors::Long(int stepNum){
  if(stepNum<3){
    stepNum=stepNum+1;
    stepLength(stepNum);
  }
  return stepNum;
}

void Motors::stepLength(int stepNum){
  if (stepNum==1){
    longStep=25;
    lenght=0.25;    
  }else if(stepNum==2){
    longStep=50;
    lenght=0.50;
  }else if(stepNum==3){
    longStep=100;
    lenght=1.0;
  }
}



/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
float Motors::ResetPositionMotorRemoteControl(class Display &dis, int motor, float axis, class IRrecv &IRremote, class decode_results &IRresult){
  Serial.println("------------ResetPositionMotorRemoteControl:------------");
  /*Select one of the 4 options:
    Up_Backward_Right
    Down_Forward_Left  
    ENTER = To save changes (Enter/OK)
    EXIT = To back
  */

  dis.reset(motor, axis);          
  auxUp=0;  //motor 1
  auxDown=0; //motor 1
  auxLeft=0;  //motor 2
  auxRight=0; //motor 2
  auxForward=0; //motor 3
  auxBackward=0;  //motor 3
  reset = false;
  stepNum = 1;
  stepLength(stepNum); //to low steps
  IRremote.resume(); // Enable receiving of the next value 
  while (reset == false){
    if (IRremote.decode(&IRresult)) {
      
      if (IRresult.value == Repeat_) { // if the value is equal to 0xFFFFFFFF
        IRresult.value = key_value; // set the value to the key value                 
      }else{
        delay(500);
      }
                     
      IRremote.resume(); // Enable receiving of the next value
      Serial.println(IRresult.value + String("  "));
      switch (IRresult.value) {
        case Up_Backward_Right:
          if(motor==1){
            axis = up(axis, true);
            dis.resetPosition(axis);
            auxUp=auxUp+1;            
          }else if(motor==2){
            axis = backward(axis, true);
            dis.resetPosition(axis);
            auxBackward=auxBackward+1;            
          }else if(motor==3){
            axis = right(axis, true);
            dis.resetPosition(axis);
            auxRight=auxRight+1;              
          }
          key_value = IRresult.value;
          //delay(100);
          break;

        case Down_Forward_Left: 
          if(motor==1){
            axis = down(axis, true);
            dis.resetPosition(axis);
            auxDown=auxDown+1;            
          }else if(motor==2){
            axis = forward(axis, true);
            dis.resetPosition(axis);
            auxForward=auxForward+1;            
          }else if(motor==3){
            axis = left(axis, true);
            dis.resetPosition(axis);
            auxLeft=auxLeft+1;              
          }
          key_value = IRresult.value;
          //delay(100);
          break;       
        
        case EXIT:  //EQ to ignore, to back
          /***** back *****/
          IRremote.resume(); // Enable receiving of the next value
          IRresult.value = 0;
          dis.AskgoBack();
          aux=1;
          while (aux == 1) {
            if (IRremote.decode(&IRresult)) {
              IRremote.resume(); // Enable receiving of the next value
              Serial.println(IRresult.value + String("-  "));
              switch (IRresult.value) { 
                case ENTER:  //PLAY to confirm exit and do not save changes
                  /***** Enter *****/
                  dis.donotSaveChangesMESSAGE();
                  if(motor==1){
                    goBack=auxDown-auxUp;
                  }else if(motor==2){
                    //goBack=auxBackward-auxForward;
                    goBack=auxForward-auxBackward;
                  }else if(motor==3){
                    goBack=auxLeft-auxRight;
                  }
                                   
                  if(goBack<0){//if goBack es negative, go to process...
                    for(int i=goBack; i<0; i++){ 
                      if(motor==1){
                        axis = down(axis, true);
                      }else if(motor==2){
                        axis = forward(axis, true);
                      }else if(motor==3){
                        axis = left(axis, true);
                      }  
                      dis.returning(axis);
                    }
                  }else{//if goBack es positive, go to process...
                    for(int i=goBack; i>0; i--){
                      if(motor==1){
                        axis = up(axis, true);
                      }else if(motor==2){
                        axis = backward(axis, true);
                      }else if(motor==3){
                        axis = right(axis, true);
                      }
                      dis.returning(axis);
                    }                          
                  }                  
                  
                  if(motor==1){
                    dis.screenOn(Conveyor, 1, axis, stepNum);  
                  }else if(motor==2){
                    dis.screenOn(LabelsX, 2, axis, stepNum);
                  }else{
                    dis.screenOn(LabelsY, 3, axis, stepNum);
                  }
                                    
                  reset = true;
                  aux = 0;
                  break;
                case EXIT:  //EQ to ignore, to back (go to screen reset position)
                  /***** back *****/
                  dis.reset(motor, axis);
                  reset = false;
                  aux = 0;
                  break;
              }
            }
            IRresult.value = 0;
          }
          key_value = 0;
          break;
          
        case ENTER:  //PLAY to save changes
          /***** Enter *****/
          IRremote.resume(); // Enable receiving of the next value
          IRresult.value = 0;
          dis.AskareYouSure();
          aux=1;
          while (aux == 1) {
            if (IRremote.decode(&IRresult)) {
              IRremote.resume(); // Enable receiving of the next value
              Serial.println(IRresult.value + String("--  "));
              switch (IRresult.value) {
                case ENTER:  //PLAY to save changes
                  /***** Enter *****/
                  axis = 0;
                  dis.successful(1);
                  delay(1000);
                  if(motor==1){
                    dis.screenOn(Conveyor, 1, axis, stepNum);  
                  }else if(motor==2){
                    dis.screenOn(LabelsX, 2, axis, stepNum);
                  }else{
                    dis.screenOn(LabelsY, 3, axis, stepNum);
                  }                  
                  reset = true;
                  aux = 0;
                  break;
                case EXIT:  //EQ to ignore, to back
                  /***** back *****/
                  dis.reset(motor, axis);
                  reset = false;
                  aux = 0;
                  break;
              }
            }
            IRresult.value = 0;
          }
          key_value = 0;
          break;                                  
      }
    }
    //IRremote.resume();
  }
  return axis;
}



float Motors::ResetPositionMotorKeyboard(class Display &dis, int motor, float axis, class USBHost &usb2, class KeyboardController &keyboard2, unsigned long &keyPress, unsigned long &keyRelease){
  Serial.println("------------ResetPositionMotorKeyboard:------------");

  dis.reset(motor, axis);          
  auxUp=0;  //motor 1
  auxDown=0; //motor 1
  auxLeft=0;  //motor 2
  auxRight=0; //motor 2
  auxForward=0; //motor 3
  auxBackward=0;  //motor 3
  reset = false;
  stepNum = 1;
  stepLength(stepNum); //to low steps
  keyPress=0;
  while (reset == false){
    usb2.Task();    
    if (keyPress!=0) {
      Serial.println(keyPress + String("  "));
      switch (keyPress) {
        case Up_Backward_Right:
          if(motor==1){
            axis = up(axis, true);
            dis.resetPosition(axis);
            auxUp=auxUp+1;            
          }else if(motor==2){
            axis = backward(axis, true);
            dis.resetPosition(axis);
            auxBackward=auxBackward+1;            
          }else if(motor==3){
            axis = right(axis, true);
            dis.resetPosition(axis);
            auxRight=auxRight+1;
          }
          break;

        case Down_Forward_Left:
          if(motor==1){
            axis = down(axis, true);
            dis.resetPosition(axis);
            auxDown=auxDown+1;            
          }else if(motor==2){
            axis = forward(axis, true);
            dis.resetPosition(axis);
            auxForward=auxForward+1;            
          }else if(motor==3){
            axis = left(axis, true);
            dis.resetPosition(axis);
            auxLeft=auxLeft+1;              
          }
          break;       
        
        case EXIT:  //EQ to ignore, to back
          /***** back *****/
          //IRremote.resume(); // Enable receiving of the next value
          //IRresult.value = 0;
          keyPress=0;
          dis.AskgoBack();
          aux=1;
          while (aux == 1) {
            usb2.Task();
            if (keyPress!=0) {  
            //if (IRremote.decode(&IRresult)) {
              //IRremote.resume(); // Enable receiving of the next value
              Serial.println(keyPress + String("-  "));
              switch (keyPress) { 
                case ENTER:  //PLAY to confirm exit and do not save changes
                  /***** Enter *****/
                  dis.donotSaveChangesMESSAGE();
                  if(motor==1){
                    goBack=auxDown-auxUp; 
                  }else if(motor==2){
                    //goBack=auxBackward-auxForward;
                    goBack=auxForward-auxBackward;
                  }else if(motor==3){
                    goBack=auxLeft-auxRight;
                  }
                                   
                  if(goBack<0){//if goBack es negative, go to process...
                    for(int i=goBack; i<0; i++){ 
                      if(motor==1){
                        axis = down(axis, true);
                      }else if(motor==2){
                        axis = forward(axis, true);
                      }else if(motor==3){
                        axis = left(axis, true);
                      }  
                      dis.returning(axis);
                    }
                  }else{//if goBack es positive, go to process...
                    for(int i=goBack; i>0; i--){
                      if(motor==1){
                        axis = up(axis, true);
                      }else if(motor==2){
                        axis = backward(axis, true);
                      }else if(motor==3){
                        axis = right(axis, true);
                      }
                      dis.returning(axis);
                    }                          
                  }                  
                  
                  if(motor==1){
                    dis.screenOn(Conveyor, 1, axis, stepNum);  
                  }else if(motor==2){
                    dis.screenOn(LabelsX, 2, axis, stepNum);
                  }else{
                    dis.screenOn(LabelsY, 3, axis, stepNum);
                  }
                                    
                  reset = true;
                  aux = 0;
                  break;
                case EXIT:  //EQ to ignore, to back (go to screen reset position)
                  /***** back *****/
                  dis.reset(motor, axis);
                  reset = false;
                  aux = 0;
                  break;
              }
            }
            //IRresult.value = 0;
            keyPress=0;
          }
          break;
          
        case ENTER:  //PLAY to save changes
          /***** Enter *****/
          //IRremote.resume(); // Enable receiving of the next value
          //IRresult.value = 0;
          keyPress=0; 
          dis.AskareYouSure();
          aux=1;
          while (aux == 1) {
            usb2.Task();
            if (keyPress!=0) {  
            //if (IRremote.decode(&IRresult)) {
              //IRremote.resume(); // Enable receiving of the next value
              Serial.println(keyPress + String("--  "));
              switch (keyPress) {
                case ENTER:  //PLAY to save changes
                  /***** Enter *****/
                  axis = 0;
                  dis.successful(1);
                  delay(1000);
                  if(motor==1){
                    dis.screenOn(Conveyor, 1, axis, stepNum);  
                  }else if(motor==2){
                    dis.screenOn(LabelsX, 2, axis, stepNum);
                  }else{
                    dis.screenOn(LabelsY, 3, axis, stepNum);
                  }                  
                  reset = true;
                  aux = 0;
                  break;
                case EXIT:  //EQ to ignore, to back
                  /***** back *****/
                  dis.reset(motor, axis);
                  reset = false;
                  aux = 0;
                  break;
              }
            }
            //IRresult.value = 0;
            keyPress=0;
          }
          break;                                  
      }
    }
    //IRremote.resume(); // Enable receiving of the next value
    keyPress=0;
  }
  return axis;
}
