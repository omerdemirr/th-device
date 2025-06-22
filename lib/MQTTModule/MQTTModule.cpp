#include "MQTTModule.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "SHT30.h"
#include "Clock.h"

extern char deviceID[];

WiFiClient espClient;
PubSubClient client(espClient);

const char *mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";

void initMQTT()
{
    client.setServer(mqtt_server, mqtt_port);
}

void mqttLoopTask(void *pvParameters)
{
    while (1)
    {
        if (WiFi.isConnected())
        {
            if (!client.connected())
            {
                char clientId[32];
                snprintf(clientId, sizeof(clientId), "ESP32-%s", deviceID);

                Serial.print("🔌 MQTT bağlanıyor...");
                if (client.connect(clientId, mqtt_username, mqtt_password))
                {
                    Serial.println("✅ MQTT bağlı!");
                }
                else
                {
                    Serial.printf("❌ MQTT bağlantı hatası [%d]\n", client.state());
                }
            }
            else
            {
                client.loop();
            }
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void mqttTask(void *pvParameters)
{
    while (1)
    {
        if (WiFi.isConnected() && client.connected())
        {
            float temp = getTemperature();
            float hum = getHumidity();
            String timestamp = getTimestamp();

            // Geçerli tarih kontrolü
            if (!timestamp.startsWith("1970"))
            {
                char tempTopic[64], humTopic[64];
                snprintf(tempTopic, sizeof(tempTopic), "TH-DEVICE/%s/temperature", deviceID);
                snprintf(humTopic, sizeof(humTopic), "TH-DEVICE/%s/humidity", deviceID);

                StaticJsonDocument<128> doc;
                char buffer[128];

                // Sıcaklık
                doc["timestamp"] = timestamp;
                doc["value"] = temp;
                serializeJson(doc, buffer);
                client.publish(tempTopic, buffer);
                Serial.printf("📤 %s => %s\n", tempTopic, buffer);

                // Nem
                doc["value"] = hum;
                serializeJson(doc, buffer);
                client.publish(humTopic, buffer);
                Serial.printf("📤 %s => %s\n", humTopic, buffer);
            }
            else
            {
                Serial.println("⏳ Saat senkronize edilmedi, MQTT atlandı.");
            }
        }
        else
        {
            Serial.println("📴 MQTT gönderimi atlandı (bağlantı yok).");
        }

        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}