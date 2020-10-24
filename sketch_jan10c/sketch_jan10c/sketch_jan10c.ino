#include <SoftwareSerial.h>

SoftwareSerial SIM900(7, 8);

String quienLlama;

void setup() {
  SIM900.begin(19200);
  Serial.begin(19200);
  delay(1000);
}

void loop() {
  quienLlama = "";

   while (SIM900.available()>0){
      delay(10);
      quienLlama += (char)SIM900.read(); 
   }
  
   Serial.print(quienLlama);

   if (quienLlama.indexOf("RING")>-1){
      delay(1000);
      String num;
      int inicio=quienLlama.indexOf('"')+1;
      int fin=inicio+12;
      num=quienLlama.substring(inicio,fin);    
      if (num=="+56995562730"){
         //Serial.print("NUMERO ACERTADO");
         SIM900.write("ATA");
      }else{
         //Serial.print("NUMERO ERRONEO");
         SIM900.write("ATH");
         
      }
  }   
}
