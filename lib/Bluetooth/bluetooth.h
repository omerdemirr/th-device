#pragma once

#include <Arduino.h>

void initBLE();

void bleSendSensorData(float temp, float hum);

void bluetoothTask(void *pvParameters);