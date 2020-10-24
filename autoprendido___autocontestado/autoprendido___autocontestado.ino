#include <String.h>

#include <SoftwareSerial.h>
String quienLlama;
String numeros[2] = {"+56995562730","+56998501404"};
//Create software serial object to communicate with SIM900
SoftwareSerial mySerial(7, 8); //SIM900 Tx & Rx is connected to Arduino #7 & #8

void SIM900power() 
/*
 Prende el GSM a traves del pin 9
 */
{
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
  delay(1000);
  digitalWrite(9, HIGH);
  delay(2000);
  digitalWrite(9, LOW);
  delay(3000);
}
void setup()
{
  /*
 Configuracion del gsm y puertos
 */
  pinMode(13, OUTPUT); // pin para mostrar cuando termina de cargar
  SIM900power();
  delay(25000); //Retardo para que encuentra a una RED
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);

  //Begin serial communication with Arduino and SIM900
  mySerial.begin(9600);

  Serial.println("Initializing...");
  delay(1000);

  mySerial.println("AT"); //Handshaking with SIM900
  updateSerial();
  mySerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
  mySerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial();
  mySerial.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial();
  digitalWrite(13, HIGH);

}

void loop()
{
  //updateSerial();
  quienLlama = "";

   while (mySerial.available()>0){ //acumula los datos de tu mamita
      delay(10);
      quienLlama += (char)mySerial.read(); 
   }
   
   Serial.print(quienLlama);

   if (quienLlama.indexOf("RING")>-1){
    Serial.write("ATA");
      /*
      delay(1000);
      String num;
      int inicio=quienLlama.indexOf('"')+1;
      int fin=inicio+12;
      num=quienLlama.substring(inicio,fin);
      for(int i=0; i<sizeof(numeros);i++){
         
      
      if (num==numeros[i]){
         //Serial.print("NUMERO ACERTADO");
         mySerial.write("ATA");
      }else{
         //Serial.print("NUMERO ERRONEO");
         mySerial.write("ATH");
         
      }

      }
      */
  }   
}

void updateSerial()
/*
traspasa los caracteres del la comunicacion serial del GSM a la del USB
 */
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
