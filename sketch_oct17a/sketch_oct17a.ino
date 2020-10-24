#include <SoftwareSerial.h>

//Create software serial object to communicate with SIM900
SoftwareSerial mySerial(7, 8); //SIM900 Tx & Rx is connected to Arduino #7 & #8
void SIM900power()
{
  Serial.print("Prendiendo... ");
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
  delay(1000);
  digitalWrite(9, HIGH);
  delay(2000);
  digitalWrite(9, LOW);
  delay(3000);
  Serial.println("done");
  
}
void setup()
{ 
  //Begin serial communication with Arduino and SIM900
  Serial.begin(9600);
  mySerial.begin(19200);
  // pin para mostrar cuando termina de cargar
  pinMode(13, OUTPUT); 
  SIM900power();
  delay(25000); //Retardo para que encuentra a una RED
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)


  
  Serial.println("Initializing...");
  delay(1000);
  mySerial.println("AT+IPR=19200");
  mySerial.println("AT"); //Handshaking with SIM900
  updateSerial();
  mySerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
  mySerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial();
  mySerial.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial();
  //mySerial.println("AT S0=2"); //autoawnser on 2 rings
  //updateSerial();
  
  updateSerial();
  digitalWrite(13, HIGH);

}

void loop()
{
  updateSerial();
}

void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (mySerial.available())
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
