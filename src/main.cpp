
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <time.h>


const char *ssid = "SENAI EDUCACIONAL";
const char *password = "info@134";

const char *BROKER = "broker.emqx.io";
const int PORTA = 1883;

const char *TOPICO_SUB = "senai134/esp32/SmartPet/comando";
const char *TOPICO_PUB = "senai134/esp32/SmartPet/status";

String id;

WiFiClient espClient;
PubSubClient mqtt(espClient);

void publishStatus();
void conectMQTT();

void setup()
{
    Serial.begin(115200);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }

    Serial.println("\nWi-Fi conectado!");

    configTime(-3 * 3600, 0, "a.st1.ntp.br", "pool.ntp.org");

    mqtt.setServer(BROKER, PORTA);
    

    id = "esp32-" + String((uint32_t)ESP.getEfuseMac(), HEX);

    Serial.printf("ID do dispositivo: %s\n", id.c_str());

    conectMQTT();
}

void loop()
{
    mqtt.loop();

    unsigned long currentMillis = millis();

    static unsigned long lastReconnect = 0;

    if (!mqtt.connected() && currentMillis - lastReconnect >= 5000)
    {
        lastReconnect = currentMillis;
        conectMQTT();
    }

    static unsigned long lastMillis = 0;

    if (currentMillis - lastMillis >= 5000)
    {
        lastMillis = currentMillis;

        if (mqtt.connected())
        {
            publishStatus();
        }
    }

  
}

void conectMQTT()
{
    Serial.printf("Conectando ao broker %s...\n", BROKER);

    if (mqtt.connect(id.c_str()))
    {
        Serial.println("Conectado ao broker!");

        mqtt.subscribe(TOPICO_SUB);

        Serial.printf("Inscrito no tópico: %s\n", TOPICO_SUB);
    }
    else
    {
        Serial.print("Erro ao conectar: ");
        Serial.println(mqtt.state());
    }
}

void publishStatus()
{
    if (!mqtt.connected())
        return;

    struct tm timeinfo;

    if (!getLocalTime(&timeinfo, 10000))
    {
        Serial.println("Erro ao obter horário NTP!");
        return;
    }

    time_t now = mktime(&timeinfo);

    time_t nextFeedingTime = now + (3 * 60 * 60);

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
    doc["nextFeedingTime"] = horarioNextFeeding;

        unsigned int currentMillis = millis();
    static unsigned long lastMillis = 0;


   
    if (currentMillis - lastMillis >= 10000)
    {
        lastMillis = currentMillis;

        doc["ledState"] = !doc["ledState"].as<bool>();
        doc["foodState"] = !doc["foodState"].as<bool>();
    }

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


