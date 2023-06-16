//********wizard!!!!****************
//To run this program I used Arduino Due
//Just EEPROM have to be connected to the board to avoid issues
//SDA: PIN 20
//SCL: PIN 21

#include "Wire.h"  //use i2c
#define disk1 0x50 //A2=0 A1=0 A0=0

float axis=0, axis1 = 0, axis2 = 0, axis3 = 0;
int INTEGER=0, DECIMAL=0;
byte data=0xFF;

void Write(unsigned int dataAddres, byte dataVal);
float ReadFLOAT(int motor);
byte Read(unsigned int dataAddres);

void setup() {
  Serial.begin(9600);

  // put your setup code here, to run once:
  Serial.println("------CERO-----------");
  delay(500);
  Write(1, 0);
  Write(2, 0);
  Write(3, 0);
  Write(4, 0);
  Write(5, 0);
  Write(6, 0); 

  axis1 = ReadFLOAT(1);
  axis2 = ReadFLOAT(2);
  axis3 = ReadFLOAT(3);
  Serial.println(axis1);
  Serial.println(axis2);
  Serial.println(axis3); 

  Serial.println("If Serial Monitor show you 0.00 3 times, the setup EEPROM is done");
}

void loop() {
  // put your main code here, to run repeatedly:

}


void Write(unsigned int dataAddres, byte dataVal){
  Wire.begin();
  Wire.beginTransmission(disk1);
  Wire.write((int)(dataAddres >> 8)); //MSB
  Wire.write((int)(dataAddres & 0xFF)); //LSB
  Wire.write(dataVal);
  Wire.endTransmission();
  delay(10);
}

float ReadFLOAT(int motor){
  
  if(motor==1){
    INTEGER=(int)Read(1);
    DECIMAL=(int)Read(2);
    axis = (float)INTEGER + (float)(DECIMAL*0.01);
  }else if(motor==2){
    INTEGER=(int)Read(3);
    DECIMAL=(int)Read(4);
    axis = (float)INTEGER + (float)(DECIMAL*0.01);  
  }else if(motor==3){
    INTEGER=(int)Read(5);
    DECIMAL=(int)Read(6);
    axis = (float)INTEGER + (float)(DECIMAL*0.01); 
  }
  
  return axis;
}

byte Read(unsigned int dataAddres){
  Wire.begin();  
  Wire.beginTransmission(disk1);
  Wire.write((int)(dataAddres >> 8)); //MSB
  Wire.write((int)(dataAddres & 0xFF)); //LSB
  Wire.endTransmission();
  Wire.requestFrom(disk1, 1);
  delay(10);
  if(Wire.available()){//////////////////==++!!--*****keep eye, posible error
    data = Wire.read();     
  }  
  return data;
}