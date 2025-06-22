#pragma once

void initSHT30();
float getTemperature();
float getHumidity();
void sensorTask(void *pvParameters);
