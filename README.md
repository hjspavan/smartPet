# 🐾 SmartPet — Sistema Inteligente de Alimentação para Pets

## 📌 Sobre o projeto

O **SmartPet** é um projeto acadêmico desenvolvido utilizando ESP32, com o objetivo de criar um sistema inteligente para auxiliar no controle da alimentação de animais de estimação.

O projeto utiliza comunicação MQTT para transmitir informações entre dispositivos ESP32, permitindo compartilhar estados, horários de alimentação e comandos por meio de um broker.

## 🎯 Objetivo

Desenvolver um sistema IoT capaz de monitorar e controlar informações relacionadas à alimentação de um pet, utilizando comunicação sem fio e troca de dados em formato JSON.

## ⚙️ Componentes utilizados

- ESP32
- LED
- Display LCD 20x4
- Botão
- Protoboard
- Jumpers

## 💻 Tecnologias e bibliotecas

| Tecnologia | Função |
|---|---|
| C++ / Arduino | Programação do ESP32 |
| Wi-Fi | Conexão com a internet |
| MQTT | Comunicação entre dispositivos |
| PubSubClient | Comunicação MQTT |
| ArduinoJson | Manipulação de dados JSON |
| NTP | Sincronização de horário |
| LiquidCrystal I2C | Controle do display LCD |

## 🌐 Comunicação MQTT

O projeto utiliza o protocolo MQTT, baseado no modelo Publisher/Subscriber.

A comunicação acontece por meio de um broker, responsável por receber as mensagens dos dispositivos e encaminhá-las aos clientes inscritos nos respectivos tópicos.

**Broker utilizado:**
- Endereço: `broker.emqx.io`
- Porta: `1883`

### Tópicos utilizados

| Tópico | Função |
|---|---|
| `senai134/esp32/SmartPet/comando` | Recebimento de comandos |
| `sen
