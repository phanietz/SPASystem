#include "Storage.h"
#include "Wire.h"  //use i2c
#define disk1 0x50 //A2=0 A1=0 A0=0

Storage::Storage(){  
}

void Storage::WriteFLOAT(int motor, float axis){
  
  INTEGER = (int)axis;
  DECIMAL = (axis - INTEGER) * 100;
  
  if(motor==1){
    Write(1, INTEGER);
    Write(2, DECIMAL);
  }else if(motor==2){
    Write(3, INTEGER);
    Write(4, DECIMAL);    
  }else if(motor==3){
    Write(5, INTEGER);
    Write(6, DECIMAL);    
  }
}

void Storage::Write(unsigned int dataAddres, byte dataVal){
  Wire.begin();
  Wire.beginTransmission(disk1);
  Wire.write((int)(dataAddres >> 8)); //MSB
  Wire.write((int)(dataAddres & 0xFF)); //LSB
  Wire.write(dataVal);
  Wire.endTransmission();
  delay(10);
}



float Storage::ReadFLOAT(int motor){
  
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


byte Storage::Read(unsigned int dataAddres){
  Wire.begin();  
  Wire.beginTransmission(disk1);
  Wire.write((int)(dataAddres >> 8)); //MSB
  Wire.write((int)(dataAddres & 0xFF)); //LSB
  Wire.endTransmission();
  Wire.requestFrom(disk1, 1);
  delay(10);
  if(Wire.available()){
    data = Wire.read();     
  }  
  return data;
}
