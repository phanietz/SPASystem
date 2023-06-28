/*************************************************************************
  File to do setup and manual connectio.
  Before start make sure to do the correct conection:
          * Pinout:
          * STATE --> pin 13
          * RXD --> TX SERIAL3
          * TXD --> RX SERIAL3
          * Key/ENABLE --> pin 12

  To see and write using Serial Monitor, you should put on BOTH NL & CR
  and keep bauds to 9600.
*************************************************************************/

//PUT 9600 BAUD TO SERIAL MONITOR
//PUT 38400 BAUD TO SERIAL BLUETOOTH

//WAITING PAIR FROM MASTER

void setup() {
  ///////Serial monitor///////
  Serial.begin(9600);

  ///////Bluetooth///////
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);  
  pinMode(15, INPUT_PULLUP);
  Serial3.begin(38400); // Default communication rate of the Bluetooth module
  Serial.println("Enter AT commands:");
}

void loop() {

  // Read from HC05 and send to Arduino
  if (Serial3.available())
    Serial.write(Serial3.read());

  // Read from serial monitor and send to HC05
  if (Serial.available()){
    Serial3.write(Serial.read());
  }
    

}
