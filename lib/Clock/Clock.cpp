#include <Arduino.h>
#include "Clock.h"
#include <time.h>

#define GMT_OFFSET_SEC 3 * 3600 // Türkiye GMT+3
#define DAYLIGHT_OFFSET_SEC 0
#define NTP_SERVER1 "pool.ntp.org"
#define NTP_SERVER2 "time.nist.gov"

void initClock()
{
    configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER1, NTP_SERVER2);

    Serial.print("🕒 Saat senkronize ediliyor");
    int retry = 0;
    while (time(nullptr) < 100000 && retry < 10)
    {
        Serial.print(".");
        delay(1000);
        retry++;
    }
    Serial.println();

    if (retry < 10)
    {
        Serial.println("✅ Saat senkronize edildi: " + getTimestamp());
    }
    else
    {
        Serial.println("❌ Saat senkronizasyonu başarısız.");
    }
}

String getCurrentDate()
{
    struct tm timeinfo;
    if (getLocalTime(&timeinfo))
    {
        char date[11];
        strftime(date, sizeof(date), "%Y-%m-%d", &timeinfo);
        return String(date);
    }
    return "----/--/--";
}

String getCurrentTime()
{
    struct tm timeinfo;
    if (getLocalTime(&timeinfo))
    {
        char time[9];
        strftime(time, sizeof(time), "%H:%M:%S", &timeinfo);
        return String(time);
    }
    return "--:--:--";
}

String getTimestamp()
{
    time_t now = time(nullptr);
    struct tm *tm_info = localtime(&now);

    char buffer[26];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
    return String(buffer);
}

String getTimeOnly()
{
    time_t now = time(nullptr);
    struct tm *tm_info = localtime(&now);

    char buffer[10];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", tm_info);
    return String(buffer);
}

String getDateOnly()
{
    time_t now = time(nullptr);
    struct tm *tm_info = localtime(&now);

    char buffer[12];
    strftime(buffer, sizeof(buffer), "%d.%m.%Y", tm_info);
    return String(buffer);
}
