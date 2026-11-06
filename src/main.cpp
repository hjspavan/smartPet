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
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include <Botao.h>

const char *ssid = "SENAI EDUCACIONAL";
const char *password = "info@134";
const char *BROKER = "broker.emqx.io";
const int PORTA = 1883;
const char *TOPICO_SUB = "senai134/esp32/SmartPet/comando"; //* onde envio o comando
const char *TOPICO_PUB = "senai134/esp32/SmartPet/status";  //* onde ele vai escutar o status
String id;

WiFiClient espClient;
PubSubClient mqtt(espClient);

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  configTime(-3 * 3600, 0, "a.st1.ntp.br", "pool.ntp.org");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("Conectado com sucesso.");
  Serial.printf("\n\rConectando ao broker %s", BROKER);

  mqtt.setServer(BROKER, PORTA);

  id = "esp32-" + String((uint32_t)ESP.getEfuseMac(), HEX);
  Serial.printf("Id do dispositivo: %s \n\r", id.c_str());

  bool conectadoAoBroker;
  conectadoAoBroker = mqtt.connect(id.c_str());

  if (conectadoAoBroker)
  {
    Serial.printf("Conectado ao Broker com sucesso.");
    mqtt.subscribe(TOPICO_SUB);
    Serial.printf("\n\rTópico: %s", TOPICO_SUB);
  }
  else
  {
    Serial.printf("Erro ao conectar: %s\n\r", BROKER);
    Serial.print("Falha: ");
    Serial.print(mqtt.state());
  }
}

void loop()
{
  mqtt.loop();

  unsigned long interval = 5000;
  unsigned long currentMillis = millis();
  static unsigned long lastMillis = 0;

  if (currentMillis - lastMillis >= interval)
  {
    lastMillis = currentMillis;

    if (mqtt.connected())
    {
      struct tm timeinfo;

      if (getLocalTime(&timeinfo))
      {
        time_t now = mktime(&timeinfo);
        time_t nextFeedingTime = now + 3 * 60 * 60; // Adiciona 3 horas ao horário atual

        struct tm nextTimeinfo;
        localtime_r(&nextFeedingTime, &nextTimeinfo);

        char horario[6];
        strftime(horario, sizeof(horario), "%H:%M", &timeinfo);

        char horarioNextFeeding[6];
        strftime(horarioNextFeeding, sizeof(horarioNextFeeding), "%H:%M", &nextTimeinfo);

        JsonDocument doc;

        doc["ledState"] = true;
        doc["foodState"] = true;
        doc["lastFeedingTime"] = horario;
        doc["nextFeedingTime"] = horarioNextFeeding; // Adiciona 3 horas ao horário atual

        String message;
        serializeJson(doc, message);

        if (mqtt.publish(TOPICO_PUB, message.c_str()))
        {
          Serial.println("Status do pet publicado!");
          Serial.println(message);
        }
        else
        {
          Serial.println("Erro ao publicar status!");
          Serial.println(mqtt.state());
        }
      }
    }
  }
}
