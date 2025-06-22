#pragma once

#include <Arduino.h>

void initWebServer(); // Web sunucuyu başlatır
void updateWebData(float temperature, float humidity, const String &dateStr, const String &timeStr);
