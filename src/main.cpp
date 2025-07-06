#include <Arduino.h>
#include <WiFi.h>
#include "Conn.h"
#include "SHT30.h"
#include "Clock.h"
#include "MQTTModule.h"
#include "ILI9341.h"
#include "bluetooth.h"
#include "webserver.h"

char deviceID[16];

void initDeviceID()
{
  uint64_t chipid = ESP.getEfuseMac();
  snprintf(deviceID, sizeof(deviceID), "%04X", (uint16_t)(chipid >> 32 ^ chipid));
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println("Sistem başlatılıyor...");
  Serial.println(getTimestamp());

  IPAddress local_IP(192, 168, 1, 200);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress dns(8, 8, 8, 8);

  WiFi.config(local_IP, gateway, subnet, dns);

  initTFT();
  initWiFi();
  initDeviceID();
  delay(2000);
  initClock();
  initSHT30();
  initMQTT();
  initBLE();
  initWebServer();

  xTaskCreate(wifiTask, "WiFiTask", 4096, NULL, 1, NULL);
  xTaskCreate(sensorTask, "SensorTask", 4096, NULL, 1, NULL);
  xTaskCreate(mqttLoopTask, "MQTTLoopTask", 4096, NULL, 1, NULL);
  xTaskCreate(mqttTask, "MQTTPublishTask", 4096, NULL, 1, NULL);
  xTaskCreatePinnedToCore(ili9341Task, "ILI9341 Task", 4096, NULL, 1, NULL, 1);
  xTaskCreate(bluetoothTask, "BluetoothTask", 4096, NULL, 1, NULL);
}

void loop()
{
}