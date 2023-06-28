#define DECODE_NEC
#include "Display.h"
#include "RemoteAccessControl.h"
#include "IRremote2.h"
#include "Storage.h"

//Do bluetooth setup before connect it with the system,



/*void RAC::pruebaRemote(class Display &dis){

  dis.screenOn("hooa", 3, 5, 1);
  delay(2500);
  delay(2500);
}
*/

RAC::RAC(){ //Constructor

}

void RAC::INITIALIZED(){
  responseAT=RESET(false);

  responseAT=INIT(false);

  responseAT=INQ(false);

}


/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
String RAC::RESET(bool mode){
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
    if(mode==true){
      Serial.println(responseAT);
    }
  }while(responseAT.indexOf("OK")==-1); //4 para OK
  //delay(1000); 
  return String(Serial.println("OK"));
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
String RAC::INIT(bool mode){
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
    }

    if(mode==true){
      Serial.println(responseAT);
      Serial.println(responseAT.indexOf("OK"));
      Serial.println(responseAT.indexOf("17"));
    }
    if(responseAT.indexOf("OK")!=-1 or responseAT.indexOf("17")!=-1){
      responseAT="done";
    }
  }while(responseAT.indexOf("done")==-1); //4 para OK
  //delay(1000);
  return String(Serial.println("OK"));
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
String RAC::INQ(bool mode){
  Serial.println("AT+INQ"); 

  if(mode==true){
    Serial.println("PUT SLAVE ON PAIRABLE mode ...");
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
    }
    if(mode==true){
      Serial.println(responseAT);
    }
  }while(responseAT.indexOf("OK")==-1); //if is 4 means only read OK and not full data

  return String(Serial.println("OK"));
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
void RAC::SendCopy(){
    Serial3.write("COPY\r\n");
    Serial3.flush();
};