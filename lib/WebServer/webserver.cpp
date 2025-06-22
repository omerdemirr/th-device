#include "webserver.h"
#include <FS.h>
#include <SPIFFS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

static float currentTemp = 0.0;
static float currentHum = 0.0;
static String currentDate = "--";
static String currentTime = "--";

void initWebServer()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("❌ SPIFFS başlatılamadı!");
        return;
    }

    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

    server.on("/data.json", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  String json = "{";
                  json += "\"temp\":" + String(currentTemp, 1) + ",";
                  json += "\"hum\":" + String(currentHum, 0) + ",";
                  json += "\"date\":\"" + currentDate + "\",";
                  json += "\"time\":\"" + currentTime + "\"";
                  json += "}";
                  request->send(200, "application/json", json); });

    server.begin();
    Serial.println("🌐 Async Web server başlatıldı");
}

void updateWebData(float temperature, float humidity, const String &dateStr, const String &timeStr)
{
    currentTemp = temperature;
    currentHum = humidity;
    currentDate = dateStr;
    currentTime = timeStr;
}