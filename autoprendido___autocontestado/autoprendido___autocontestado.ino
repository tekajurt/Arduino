#include <String.h>
#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0])

#include <SoftwareSerial.h>
String quienLlama;
String numeros[2] = {"+56995562730","+56957503575"};
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
  //pinMode(13, OUTPUT); // pin para mostrar cuando termina de cargar
  
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
    //SIM900power();

  Serial.println("Prendiendo");
  //Begin serial communication with Arduino and SIM900
  mySerial.begin(9600);
  for(int i=0;i<25;i++){
      //Serial.println(i);
      //delay(1000);
  }
  Serial.println("Initializing...");
  delay(1000);

  mySerial.println("AT"); //Handshaking with SIM900
  updateSerial();
  mySerial.println("ATZ");
  updateSerial();
  mySerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
  mySerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial();
  mySerial.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial();
  digitalWrite(13, HIGH);

}

void ChechAT() {
  
}
void checkCall() {
  quienLlama = "";
  
  while (mySerial.available()>0){ //acumula los datos de tu mamita
    delay(10);
    quienLlama += (char)mySerial.read(); 
  }
  
  Serial.print(quienLlama);
  
  if (quienLlama.indexOf("RING")>-1){
    //mySerial.println("ATA");
  
    String num;
    int inicio=quienLlama.indexOf('"')+1;
    int fin=inicio+12;
    num=quienLlama.substring(inicio,fin);
    Serial.println(num);
    for (int i = 0; i<ARRAY_SIZE(numeros);i++){
      if(num==numeros[i]){
        mySerial.println("ata");
        return;
      }else{
        mySerial.println("Siguiente");
      }
    }
    mySerial.println("ath");
   
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
void loop()
{
  //updateSerial();
  checkCall();
  
}
