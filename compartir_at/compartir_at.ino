#include <SoftwareSerial.h>

SoftwareSerial gprsSerial(7,8);  // RX, TX pins para el GPRS Shield SIM900

void setup() {
  Serial.begin(9600);
  gprsSerial.begin(9600);

  delay(2000);

  Serial.println("Configurando GPRS...");

  sendATCommand("AT");  // Enviar comando AT para verificar la comunicación
  delay(1000);

  sendATCommand("AT+CMGF=1");  // Establecer el modo de texto para enviar mensajes SMS
  delay(1000);

  sendATCommand("AT+CNMI=2,2,0,0,0");  // Configurar el manejo de mensajes entrantes
  delay(1000);

  Serial.println("GPRS listo!");
}

void loop() {
  // Esperar a que llegue un mensaje
  if (gprsSerial.available()) {
    String message = gprsSerial.readString();
    if(message.indexOf("RING")>-1){
      sendATCommand("ATA");
    }
    Serial.println("Mensaje recibido: " + message);
  }
}

void sendATCommand(String command) {
  gprsSerial.println(command);
  delay(1000);

  while (gprsSerial.available()) {
    Serial.write(gprsSerial.read());
  }
}
