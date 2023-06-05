#define DECODE_NEC
#include "Display.h"
#include "RemoteControl.h"
#include "IRremote2.h"
#include "Storage.h"


Remote::Remote(){ //Constructor

}

void Remote::pruebaRemote(class Display &dis){

  dis.screenOn("hooa", 3, 5, 1);
  delay(2500);
  delay(2500);
}
