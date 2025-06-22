#pragma once

#include <Adafruit_ILI9341.h>

#define TFT_CS 5
#define TFT_DC 2
#define TFT_RST 4
#define TFT_MOSI 23
#define TFT_CLK 18
#define TFT_MISO -1

// TFT nesnesi
extern Adafruit_ILI9341 tft;

void initTFT();
void updateTFT(float temperature, float humidity, const char *timeStr, const char *dateStr);

void ili9341Task(void *pvParameters);