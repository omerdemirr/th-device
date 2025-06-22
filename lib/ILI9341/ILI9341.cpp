#include "ILI9341.h"
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

static float lastTemp = -1000.0f;
static float lastHum = -1000.0f;
static String lastTime = "";
static String lastDate = "";

void initTFT()
{
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(30, 20);
    tft.println("OMER DEMIR - 2320221036");
}

void clearTextArea(int x, int y, int w, int h)
{
    tft.fillRect(x, y, w, h, ILI9341_BLACK);
}

void updateTFT(float temperature, float humidity, const char *timeStr, const char *dateStr)
{
    String timeStrS = String(timeStr);
    String dateStrS = String(dateStr);

    tft.setTextSize(3);

    // --- Sıcaklık ---
    if (temperature != lastTemp)
    {
        lastTemp = temperature;
        tft.setTextColor(ILI9341_RED, ILI9341_BLACK); // Arka planı siyah yap
        int tempX = (tft.width() - (String("Temp: " + String(temperature, 1) + " C").length() * 6 * 3)) / 2;
        tft.setCursor(max(tempX, 0), 60);
        tft.printf("Temp: %.1f C", temperature);
    }

    // --- Nem ---
    if (humidity != lastHum)
    {
        lastHum = humidity;
        tft.setTextColor(ILI9341_BLUE, ILI9341_BLACK);
        int humX = (tft.width() - (String("Humidity: " + String(humidity, 0) + " %").length() * 6 * 3)) / 2;
        tft.setCursor(max(humX, 0), 110);
        tft.printf("Humidity: %.0f %%", humidity);
    }

    // --- Tarih ---
    if (dateStrS != lastDate)
    {
        lastDate = dateStrS;
        tft.setTextSize(2);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        int dateX = (tft.width() - (strlen(dateStr) * 12)) / 2;
        tft.setCursor(max(dateX, 0), 180);
        tft.println(dateStr);
    }

    // --- Saat ---
    if (timeStrS != lastTime)
    {
        lastTime = timeStrS;
        tft.setTextSize(2);
        tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        int timeX = (tft.width() - (strlen(timeStr) * 12)) / 2;
        tft.setCursor(max(timeX, 0), 210);
        tft.println(timeStr);
    }
}

void ili9341Task(void *pvParameters)
{
    while (1)
    {
        extern float getTemperature();
        extern float getHumidity();
        extern String getCurrentTime();
        extern String getCurrentDate();

        float temp = getTemperature();
        float hum = getHumidity();
        String timeStr = getCurrentTime();
        String dateStr = getCurrentDate();

        updateTFT(temp, hum, timeStr.c_str(), dateStr.c_str());

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}