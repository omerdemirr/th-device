#include "SHT30.h"
#include <Wire.h>
#include <Adafruit_SHT31.h>
#include "Clock.h"
#include "webserver.h"

Adafruit_SHT31 sht = Adafruit_SHT31();
float temperature = 0.0;
float humidity = 0.0;

void initSHT30()
{
    Wire.begin(21, 22);
    if (!sht.begin(0x44))
    {
        Serial.println("❌ SHT30 sensörü bulunamadı!");
    }
    else
    {
        Serial.println("✅ SHT30 başlatıldı.");
    }
}

float getTemperature()
{
    return temperature;
}

float getHumidity()
{
    return humidity;
}

void sensorTask(void *pvParameters)
{
    while (true)
    {
        temperature = sht.readTemperature();
        humidity = sht.readHumidity();

        String tempStr = String(temperature, 1);
        String humStr = String(humidity, 0);

        String date = getCurrentDate();
        String time = getCurrentTime();

        Serial.print("Temp: ");
        Serial.println(temperature);
        Serial.print("Humidity: ");
        Serial.println(humidity);

        updateWebData(temperature, humidity, getCurrentDate(), getCurrentTime());

        vTaskDelay(pdMS_TO_TICKS(1000));
        continue;
    }
}