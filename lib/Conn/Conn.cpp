#include "Conn.h"
#include <WiFi.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>

AsyncWebServer Server(80);
DNSServer dns;
bool wifiConnected = false;

void initWiFi()
{
    WiFi.mode(WIFI_STA);
    AsyncWiFiManager wifiManager(&Server, &dns);

    bool res = wifiManager.autoConnect("TH-DEVICE");

    if (!res)
    {
        Serial.println("WiFiManager: Bağlantı başarısız. Reset atılıyor...");
        delay(3000);
        ESP.restart();
    }
    else
    {
        Serial.print("WiFi Bağlandı: ");
        Serial.println(WiFi.localIP());
        wifiConnected = true;
    }
}

bool isWiFiConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

void wifiTask(void *pvParameters)
{
    while (1)
    {
        wifiConnected = WiFi.status() == WL_CONNECTED;
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}