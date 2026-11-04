#include <Arduino.h>
#include <wiFi.h>

const char* ssid = "SENAI EDUCAIONAL";
const char* password = "info@134";



void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  if (WL_CONNECTED)
  {
    Serial.println("Conectado com sucesso!");
  }
  else
  {
    Serial.println("Falha na conexão!");
    Serial.println(WiFi.status());
  }
  


}

void loop() {
  
}

