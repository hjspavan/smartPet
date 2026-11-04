//*============================================|
//* Projeto: projeto mqtt - em dupla           |
//* Descrição: Alimentação de pet              |
//* Autor: Gabriel Pedrão e Henrique Jordão    |
//* Data: 04/11/2026                           |
//* Componentes:                               |
//* - ESP 32                                   |
//* - LED                                      |
//* - LCD 20x4                                 |
//* - Botão                                    |
//*============================================|



#include <Arduino.h>
#include <wiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "SENAI EDUCAIONAL";
const char* password = "info@134";

WiFiClient espClient;
PubSubClient mqtt(espClient);


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
  mqtt.loop();
}

void tratarJson(String message)
{
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, message);

}