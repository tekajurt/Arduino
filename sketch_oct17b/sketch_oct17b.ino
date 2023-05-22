#include <SoftwareSerial.h>
int incoming_char=0;
int Incomingch;
String data,Fdata;
//Create software serial object to communicate with SIM900
SoftwareSerial mySerial(7, 8); //SIM900 Tx & Rx is connected to Arduino #7 & #8
void SIM900power()
{
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
  delay(1000);
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
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
void setup()
{
  Serial.begin(9600);
  Serial.println("Partiendo");
  SIM900power();
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  ;

  //Begin serial communication with Arduino and SIM900
  mySerial.begin(9600);

  Serial.println("Initializing...");
  delay(1000);
  
  mySerial.write("AT"); //Handshaking with SIM900
  updateSerial();
  mySerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
  mySerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial();
  mySerial.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial();
  digitalWrite(13, HIGH);

}

void check_Incoming()
{
    if(mySerial.available()) //If GSM is saying something
    {
   Incomingch = mySerial.read(); // Listen to it and store in this variable

  if (Incomingch == 10 || Incomingch ==13) //If it says space (10) or Newline (13) it means it has completed one word
  {Serial.println(data);  Fdata =data; data = ""; } //Print the word and clear the variable to start fresh
  else
  {
  String newchar = String (char(Incomingch)); //convert the char to string by using string objects
  data = data +newchar; // After converting to string, do string concatenation
  }
  }
}

void loop()
{
  if (Fdata == "RING") //If the GSM module says RING
  {
  delay(5000); //wait for 5sec to create 3 ring delay.
  mySerial.write ("ATA\r\n"); //Answer the call
  Serial.println ("Placed Received");  //Used for debugging
  while(Fdata != "OK") //Until call successfully answered
  {check_Incoming(); //Read what GSM modue is saying
  Serial.println ("Playing Recorded message");  //Used for debugging

  //Play the recored voice message
  delay(500);
  digitalWrite(8, HIGH); //Go high 
  delay(200);     // wait for 200 msec              
  digitalWrite(8, LOW); //Go low
  }
  updateSerial();
  
  
}
}
