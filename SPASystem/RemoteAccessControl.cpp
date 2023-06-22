#define DECODE_NEC
#include "Display.h"
#include "RemoteAccessControl.h"
#include "IRremote2.h"
#include "Storage.h"

//Do bluetooth setup before connect it with the system,

RAC::RAC(){ //Constructor

}

/*void RAC::pruebaRemote(class Display &dis){

  dis.screenOn("hooa", 3, 5, 1);
  delay(2500);
  delay(2500);
}
*/

void RAC::statusPAIR(){
  responseAT=RESET();

  responseAT=INIT();

  //responseAT=INQ(false);

}


/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
String RAC::RESET(){
  Serial.println("AT+RESET"); 
  responseAT="";
  do{
    responseAT="";
    Serial3.write("AT+RESET\r\n");
    Serial3.flush();
    delay(500);
    while(Serial3.available()>0){
      a = char(Serial3.read());
      responseAT += a; 
    }
    Serial.println(responseAT);
  }while(responseAT.indexOf("OK")!=-1); //4 para OK
  
  //responseAT=responseAT.substring(0, responseAT.length()-2); //remove \r\n from command string    
  return String(Serial.println(responseAT));
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
String RAC::INIT(){
  Serial.println("AT+INIT");
  responseAT="";
  do{
    responseAT="";
    Serial3.write("AT+INIT\r\n");
    Serial3.flush();
    delay(500);
    while(Serial3.available()>0){
      a = char(Serial3.read());
      responseAT += a;    
      Serial.println(responseAT);
    }
    Serial.println(responseAT);
    /*if(responseAT.length()==12){
      //Serial.println(responseAT);
      responseAT="OK\r\n";
    }*/
  }while(responseAT.indexOf("OK")!=-1 or responseAT.indexOf("17")!=-1); //4 para OK

  //responseAT=responseAT.substring(0, responseAT.length()-2); //remove \r\n from command string    
  return String(Serial.println(responseAT));
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
String RAC::INQ(bool mode){
  Serial.println("AT+INQ"); 

  if(mode==true){
    Serial.println("________________________________");
    Serial.println("PUT on PAIR mode ...");
    Serial.println("________________________________");
  }

  responseAT="";
  do{        
    responseAT="";
    Serial3.write("AT+INQ\r\n");
    Serial3.flush();
    delay(500);
    while(Serial3.available()>0){
      a = char(Serial3.read());
      responseAT += a;
      Serial.println(responseAT);
    }
    Serial.println(responseAT);
  }while(responseAT.indexOf("OK")!=-1); //if is 4 means only read OK and not full data
  
  //responseAT=responseAT.substring(0, responseAT.length()-2); //remove \r\n from command string

  return String(Serial.println(responseAT));
};